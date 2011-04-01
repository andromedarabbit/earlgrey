using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Data.SqlClient;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.SqlServer.Management.Common;
using Microsoft.SqlServer.Management.Smo;

namespace MSBuild.Earlgrey.Tasks.SqlServer2008
{

    public class GenerateSqlScripts : AbstractTask
    {
        private string _outputDir;

        #region public properties

        public string ConnectionString { get; set; }
        public string Server { get; set; }
        public string UserName { get; set; }
        public string Password { get; set; }

        [Required]
        public string Database { get; set; }

        public bool AllowSystemObjects { get; set; }

        public bool ClusteredIndexes { get; set; }
        public bool ContinueScriptingOnError { get; set; }
        public bool ConvertUserDefinedDataTypesToBaseType { get; set; }

        public bool ExtendedProperties { get; set; }
        public bool FullTextIndexes { get; set; }

        public bool IncludeIfNotExists { get; set; }
        public bool IncludeHeaders { get; set; }
        public bool Indexes { get; set; }

        public bool NoCollation { get; set; }
        public bool NonClusteredIndexes { get; set; }

        public bool SchemaQualify { get; set; }
        public bool Triggers { get; set; }


        /*
        public bool CopyAllObjects { get; set; }
        public bool CopySchema { get; set; }        
        public bool CopyAllDatabaseTriggers { get; set; }
         * */

        public string OutputDir
        {
            get
            {
                if (string.IsNullOrEmpty(_outputDir))
                    _outputDir = AppDomain.CurrentDomain.BaseDirectory;
                return _outputDir;
            }
            set { _outputDir = value; }
        }

        public string OutputFileName { get; set; }

        public bool Overwrite { get; set; }
        public bool CopyData { get; set; }

        // public string[] Includes

        #endregion // public properties

        public GenerateSqlScripts()
        {
            this.AllowSystemObjects = false;
            this.IncludeHeaders = false;
        }

        protected override bool ValidateParameters()
        {
            return true;
        }

        protected override bool ExecuteCommand()
        {
            ServerConnection serverConnection = GetServerConnection();
            Server server = new Server(serverConnection);
            Database database = server.Databases[Database];


            Scripter scripter = new Scripter(server);
            scripter.Options = Options;

            if (ScriptSmoCollectionBase(scripter, database.Tables) == false)
                return false;

            if (ScriptSmoCollectionBase(scripter, database.Views) == false)
                return false;

            if (ScriptSmoCollectionBase(scripter, database.StoredProcedures) == false)
                return false;

            if (ScriptSmoCollectionBase(scripter, database.Assemblies) == false)
                return false;


            return true;
        }



        internal ScriptingOptions Options
        {
            get
            {
                ScriptingOptions options = new ScriptingOptions();
                options.AllowSystemObjects = this.AllowSystemObjects;
                options.ClusteredIndexes = this.ClusteredIndexes;
                options.ExtendedProperties = this.ExtendedProperties;
                options.ContinueScriptingOnError = this.ContinueScriptingOnError;
                options.ConvertUserDefinedDataTypesToBaseType = this.ConvertUserDefinedDataTypesToBaseType;
                options.IncludeIfNotExists = this.IncludeIfNotExists;
                options.IncludeHeaders = this.IncludeHeaders;
                options.Indexes = this.Indexes;
                options.FullTextIndexes = this.FullTextIndexes;
                options.NoCollation = this.NoCollation;
                options.NonClusteredIndexes = this.NonClusteredIndexes;
                options.SchemaQualify = this.SchemaQualify;
                options.Triggers = this.Triggers;
                // options.ScriptData = this.CopyData;

                options.ToFileOnly = true;
                options.AnsiFile = false;
                options.Encoding = Encoding.Unicode;


                if (SingleOutputFile)
                {
                    if (PrepareOutputFile(OutputFilePath) == false)
                        return null;

                    options.FileName = OutputFilePath;
                }
                options.AppendToFile = SingleOutputFile == true;
                return options;
            }
        }


        private bool PrepareOutputFile(string outputFilePath)
        {
            bool fileExists = File.Exists(outputFilePath);
            if (fileExists == true && Overwrite == false)
            {
                Log.LogError("File already exists : {0}", outputFilePath);
                return false;
            }

            if (fileExists == true && Overwrite == true)
            {
                File.Delete(outputFilePath);
            }

            string dir = Path.GetDirectoryName(outputFilePath);
            if (Directory.Exists(dir) == false)
                Directory.CreateDirectory(dir);


            return true;
        }

        internal string OutputFilePath
        {
            get { return Path.Combine(OutputDir, OutputFileName); }
        }


        private bool ScriptSmoCollectionBase(Scripter scripter, SmoCollectionBase collection)
        {
            foreach (ScriptNameObjectBase item in collection)
            {
                if (ScriptScriptNameObjectBase(scripter, item) == false)
                    return false;
            }
            return true;
        }

        private bool ScriptScriptNameObjectBase(Scripter scripter, ScriptNameObjectBase obj)
        {
            if (this.AllowSystemObjects == false && IsSystemObject(obj) == true)
                return true;

            if (SingleOutputFile == false)
            {
                string filePath = GetOutputFilePath(obj);

                if (PrepareOutputFile(filePath) == false)
                    return false;

                scripter.Options.FileName = filePath;
            }

            Script(scripter, obj);

            return true;
        }

        private void Script(Scripter scripter, ScriptNameObjectBase obj)
        {            
            if (obj is Table && this.CopyData == true)
            {
                scripter.Options.ScriptData = true;
                
                IEnumerable<string> dataScripts = scripter.EnumScript(new SqlSmoObject[] { obj });
                Debug.Assert(dataScripts != null);
                
                scripter.Options.ScriptData = false;
                return;
            }
         
            scripter.Script(new SqlSmoObject[] { obj });            
        }

        private string GetOutputFilePath(ScriptNameObjectBase obj)
        {
            if (SingleOutputFile == true)
                return null;

            string fileName = string.Empty;
            if (obj is ScriptSchemaObjectBase)
                fileName = ((ScriptSchemaObjectBase)obj).Schema + ".";
            fileName += obj.Name + GetExtension(obj);

            return Path.Combine(
                OutputDir, fileName
                );
        }




        private static bool IsSystemObject(ScriptNameObjectBase obj)
        {
            var table = obj as Table;
            if (table != null)
                return table.IsSystemObject;

            var view = obj as View;
            if (view != null)
                return view.IsSystemObject;

            var storedProcedure = obj as StoredProcedure;
            if (storedProcedure != null)
                return storedProcedure.IsSystemObject;


            var sqlAssembly = obj as SqlAssembly;
            if (sqlAssembly != null)
                return sqlAssembly.Owner.Equals("sys", StringComparison.CurrentCultureIgnoreCase);


            throw new InvalidArgumentException();
        }

        private static string GetExtension(NamedSmoObject obj)
        {
            Trace.Assert(
                obj is View
                || obj is Table
                || obj is StoredProcedure
                || obj is SqlAssembly
            );
            string name = obj.GetType().Name;
            return string.Format(".{0}.sql", name);
        }

        private bool SingleOutputFile
        {
            get { return string.IsNullOrEmpty(OutputFileName) == false; }
        }

        private ServerConnection GetServerConnection()
        {
            if (string.IsNullOrEmpty(ConnectionString) && string.IsNullOrEmpty(Server))
                return new ServerConnection();

            if (string.IsNullOrEmpty(ConnectionString) == false)
                new ServerConnection(new SqlConnection(ConnectionString));

            if (string.IsNullOrEmpty(UserName) == true)
                return new ServerConnection(Server);

            return new ServerConnection(Server, UserName, Password);
        }

    }
}
