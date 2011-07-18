using System;
using System.Collections;
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

    public class GenerateSqlScripts : AutoResolveTask
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

        public string EncodingOrCodePage { get; set; }

        public bool ExtendedProperties { get; set; }
        public bool FullTextIndexes { get; set; }

        public bool IncludeDatabaseContext { get; set; }
        public bool IncludeIfNotExists { get; set; }
        public bool IncludeHeaders { get; set; }
        public bool Indexes { get; set; }

        public bool NoFileGroup { get; set; }

        public bool NoCollation { get; set; }
        public bool NonClusteredIndexes { get; set; }

        public bool Permissions { get; set; }

        public bool ScriptDrops { get; set; }
        public bool SchemaQualify { get; set; }
        public bool ScriptSchema { get; set; }
        public bool Triggers { get; set; }

        public bool AllConstraints { get; set; }


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

        public ITaskItem[] IncludeTables { get; set; }
        public ITaskItem[] ExcludeTables { get; set; }

        /*
        public bool CopyAllObjects { get; set; }        
        public bool CopyAllDatabaseTriggers { get; set; }         
        public bool CopyAllSchemas { get; set; }
         * * */
        public bool CopyData { get; set; }

        // public string[] Includes

        #endregion // public properties

        internal Encoding InternalEncoding
        {
            get
            {
                if (string.IsNullOrEmpty(EncodingOrCodePage) == true)
                {
                    return Encoding.Unicode;
                }

                int codePage = 0;
                if (int.TryParse(EncodingOrCodePage, out codePage) == true)
                {
                    return Encoding.GetEncoding(codePage);
                }

                return Encoding.GetEncoding(EncodingOrCodePage);
            }
        }
        
        public GenerateSqlScripts()
        {
            this.AllConstraints = true;
            this.ScriptSchema = true;
            this.NoFileGroup = true;
        }

        protected override bool ValidateParameters()
        {            
            if (IsIncludeTablesSpecified && IsExcludeTablesSpecified)
            {
                Log.LogError("An option 'IncludeTables' can not be used with an option 'ExcludeTables'.");
                return false;
            }

            if (ScriptSchema == false && CopyData == false)
            {
                Log.LogError("Either of 'ScriptSchema' or 'CopyData' should be 'true'.");
                return false;
            }
            return true;
        }

      


        protected override bool ExecuteCommand()
        {
            ServerConnection serverConnection = GetServerConnection();
            Server server = new Server(serverConnection);
            Database database = server.Databases[Database];


            Scripter scripter = new Scripter(server);
            scripter.Options = Options;

  
            IEnumerable tables = GetTables(database.Tables);
            if (ScriptSmoCollectionBase(scripter, tables) == false)
                return false;

            if (ScriptSmoCollectionBase(scripter, database.Views) == false)
                return false;

            if (ScriptSmoCollectionBase(scripter, database.StoredProcedures) == false)
                return false;

            if (ScriptSmoCollectionBase(scripter, database.Assemblies) == false)
                return false;

            // if (ScriptSmoCollectionBase(scripter, database.Schemas) == false)
                // return false;

            if (ScriptSmoCollectionBase(scripter, database.Users) == false)
                return false;

            return true;
        }

        private bool IsIncludeTablesSpecified
        {
            get { return (IncludeTables != null && IncludeTables.Length > 0); }
        }

        private bool IsExcludeTablesSpecified
        {
            get { return (ExcludeTables != null && ExcludeTables.Length > 0); }
        }

        private IEnumerable GetTables(SmoCollectionBase collection)
        {
            Debug.Assert(IsIncludeTablesSpecified == false || IsExcludeTablesSpecified == false);

            if(IsIncludeTablesSpecified)
            {
                return GetMatchingTables(collection, IncludeTables);
            }

            if(IsExcludeTablesSpecified)
            {
                return GetNotMatchingTables(collection, ExcludeTables);
            }

            return collection;
        }

        private static IEnumerable<ScriptNameObjectBase> GetNotMatchingTables(SmoCollectionBase collection, IEnumerable<ITaskItem> names)
        {
            return from ScriptNameObjectBase item in collection
                   from name in names
                   where TableNameEquals(item, name) == false
                   select item;
        }

        private static IEnumerable<ScriptNameObjectBase> GetMatchingTables(SmoCollectionBase collection, IEnumerable<ITaskItem> names)
        {
            return from ScriptNameObjectBase item in collection
                   from name in names
                   where TableNameEquals(item, name) == true
                   select item;
        }


        private static bool TableNameEquals(ScriptNameObjectBase obj1, ITaskItem obj2)
        {
            string name1 = string.Empty;
            if (obj1 is ScriptSchemaObjectBase)
                name1 = ((ScriptSchemaObjectBase)obj1).Schema + ".";
            name1 += obj1.Name;

            return TableNameEquals(name1, obj2.ItemSpec);
        }

        private static bool TableNameEquals(string name1, string name2)
        {
            if(string.IsNullOrEmpty(name1) || string.IsNullOrEmpty(name2))
                return false;

            string firstName = name1.Trim().Replace("[", string.Empty).Replace("]", string.Empty).ToLower();
            string secondName = name2.Trim().Replace("[", string.Empty).Replace("]", string.Empty).ToLower();
            return firstName == secondName;
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
                options.IncludeDatabaseContext = this.IncludeDatabaseContext;
                options.IncludeIfNotExists = this.IncludeIfNotExists;
                options.IncludeHeaders = this.IncludeHeaders;
                options.Indexes = this.Indexes;
                options.FullTextIndexes = this.FullTextIndexes;
                options.NoCollation = this.NoCollation;
                options.NonClusteredIndexes = this.NonClusteredIndexes;
                options.NoFileGroup = this.NoFileGroup;
                options.Permissions = this.Permissions;
                options.ScriptDrops = this.ScriptDrops;
                options.SchemaQualify = this.SchemaQualify;
                options.ScriptSchema = this.ScriptSchema;
                options.Triggers = this.Triggers;
                
                options.DriAllConstraints = this.AllConstraints;
                
                // options.ScriptData = this.CopyData;

                options.ToFileOnly = true;
                options.AnsiFile = false;
                options.Encoding = InternalEncoding;


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

        private bool ScriptSmoCollectionBase(Scripter scripter, IEnumerable<ScriptNameObjectBase> collection)
        {
            return ScriptSmoCollectionBase(scripter, collection as IEnumerable);
        }

        private bool ScriptSmoCollectionBase(Scripter scripter, IEnumerable collection)
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

            // 마지막 GO 명령 앞에 /r/n 갯수가 매번 달라지는 문제를 강제로 고치는 코드
            if (SingleOutputFile == false && File.Exists(scripter.Options.FileName))
            {
                string text = string.Empty;
                using (StreamReader sr = new StreamReader(scripter.Options.FileName, scripter.Options.Encoding))
                {
                    text = sr.ReadToEnd();                  
                }

                using (StreamWriter sw = new StreamWriter(scripter.Options.FileName, false, scripter.Options.Encoding))
                {
                    string textTrimmed = TrimLineBreaks(text) + "\r\n\r\nGO\r\n";
                    sw.Write(textTrimmed);
                }
            }

            return true;
        }

        internal static string TrimLineBreaks(string text)
        {
            if (string.IsNullOrEmpty(text))
                return text;

            int lastIndexOfGo = text.LastIndexOf("GO", StringComparison.CurrentCultureIgnoreCase);
            if (lastIndexOfGo < 0)
                lastIndexOfGo = text.Length;

            int index = lastIndexOfGo;
            while ((index - 2) > -1)
            {
                string lastTwoChars = text.Substring(index - 2, 2);
                if (lastTwoChars != "\r\n")
                    break;

                index = index - 2;
            }

            if (index < 0)
                return text;

            return text.Substring(0, index);
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
         
            if(this.ScriptSchema == false)
                return;

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

            //var schema = obj as Schema;
            //if (schema != null)
            //    return schema.Owner.Equals("sys", StringComparison.CurrentCultureIgnoreCase);

            var user = obj as User;
            if (user != null)
                return user.IsSystemObject;

            throw new InvalidArgumentException();
        }

        private static string GetExtension(NamedSmoObject obj)
        {
            Trace.Assert(
                obj is View
                || obj is Table
                || obj is StoredProcedure
                || obj is SqlAssembly
                // || obj is Schema
                || obj is User
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
                return new ServerConnection(new SqlConnection(ConnectionString));

            if (string.IsNullOrEmpty(UserName) == true)
                return new ServerConnection(Server);

            return new ServerConnection(Server, UserName, Password);
        }

    }
}
