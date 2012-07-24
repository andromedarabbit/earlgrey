using System;
using System.Collections;
using System.Collections.Generic;
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

    /// <summary>
    /// Getting database objects int the SQL script files. This is the MSBuild version of SQL Server's DTS feature.
    /// </summary>
    /// <remarks>
    /// <see cref="GenerateSqlScripts"/> can get the various types of database objects into the .sql files. 
    /// This is very useful when you want to backup or version-control the database.
    /// 
    /// Supported environments are:
    /// 
    /// <list type="bullet">
    /// <item>
    /// <description>Microsoft SQL Server 2005</description>
    /// </item>
    /// <item>
    /// <description>Microsoft SQL Server 2008</description>
    /// </item>
    /// </list>
    /// 
    /// There are several combinations of how to connect to a SQL Server.
    /// <list type="bullet">
    /// <item>
    /// <description>If you do not specify the both of <see cref="ConnectionString"/> and <see cref="Server"/>, this class will connect to the local instance using the trusted connection.</description>
    /// </item>
    /// <item>
    /// <description>If you specify the <see cref="ConnectionString"/>, this class will use it.</description>
    /// </item>
    /// <item>
    /// <description>If you specify the <see cref="Server"/> without <see cref="UserName"/>, this class will connect to the <see cref="Server"/> using the trusted connection.</description>
    /// </item>
    /// <item>
    /// <description>If you specify the <see cref="Server"/> and <see cref="UserName"/>/<see cref="Password"/>, this class will use it.</description>
    /// </item>
    /// </list>
    /// 
    /// Most of the properties refers to <see cref="Microsoft.SqlServer.Management.Smo.ScriptingOptions"/> class. See http://msdn.microsoft.com/en-us/library/microsoft.sqlserver.management.smo.scriptingoptions.aspx for more details.
    /// </remarks>
    /// <example>
    /// <code title="Scripts all the object schemas from the target database." lang="xml" source=".\Samples\msbuild-GenerateSqlScripts-only-for-schemas.xml" />
    /// <code title="Scripts all the table records from the target database." lang="xml" source=".\Samples\msbuild-GenerateSqlScripts-only-for-table-records.xml" />
    /// <code title="Scripts all the object schemas with all the table records from the target database." lang="xml" source=".\Samples\msbuild-GenerateSqlScripts-all.xml" />
    /// </example>
    public class GenerateSqlScripts : AutoResolveTask
    {
        private string _outputDir;

        #region public properties

        /// <summary>
        /// Gets or sets the connection string.
        /// </summary>
        /// <value>The connection string.</value>
        /// <remarks></remarks>
        public string ConnectionString { get; set; }

        /// <summary>
        /// Gets or sets the server address.
        /// </summary>
        /// <value>The server address.</value>
        /// <remarks></remarks>
        public string Server { get; set; }

        /// <summary>
        /// Gets or sets the name of the user.
        /// </summary>
        /// <value>The name of the user.</value>
        /// <remarks></remarks>
        public string UserName { get; set; }

        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>The password.</value>
        /// <remarks></remarks>
        public string Password { get; set; }

        /// <summary>
        /// [Required] Gets or sets the database.
        /// </summary>
        /// <value>The database.</value>
        /// <remarks></remarks>
        [Required]
        public string Database { get; set; }

        /// <summary>
        /// If you want to script system objects too, set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>s
        /// <value>Gets or sets a value indicating whether to script system objects.</value>
        /// <remarks></remarks>
        public bool AllowSystemObjects { get; set; }

        /// <summary>
        /// if you want to script clustered indexes, set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to script clustered indexes.</value>
        /// <remarks></remarks>
        public bool ClusteredIndexes { get; set; }

        /// <summary>
        /// if you do not want to stop scripting on error, set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to continue scripting on error].</value>
        /// <remarks></remarks>
        public bool ContinueScriptingOnError { get; set; }
        
        /// <summary>
        /// If you want to convert user defined data types to base types, set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to convert user defined data types to base types.</value>
        /// <remarks></remarks>
        public bool ConvertUserDefinedDataTypesToBaseType { get; set; }

        /// <summary>
        /// Gets or sets the encoding type / code page of the target script files.
        /// </summary>
        /// <value>The encoding type or code page.</value>
        /// <remarks></remarks>
        public string EncodingOrCodePage { get; set; }

        /// <summary>
        /// If you want to script [extended properties], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to script [extended properties]. </value>
        /// <remarks></remarks>
        public bool ExtendedProperties { get; set; }

        /// <summary>
        /// If you want to script [full text indexes], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to script [full text indexes]. </value>
        /// <remarks></remarks>
        public bool FullTextIndexes { get; set; }

        /// <summary>
        /// If you want to [include database context], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to [include database context]. </value>
        /// <remarks></remarks>
        public bool IncludeDatabaseContext { get; set; }

        /// <summary>
        /// If you want to [include if not exists], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to [include if not exists]. </value>
        /// <remarks></remarks>
        public bool IncludeIfNotExists { get; set; }

        /// <summary>
        /// If you want to [include headers], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to [include headers]. </value>
        /// <remarks></remarks>
        public bool IncludeHeaders { get; set; }       

        /// <summary>
        /// If you want to script [indexes], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to script [indexes]. </value>
        /// <remarks></remarks>
        public bool Indexes { get; set; }

        /// <summary>
        /// If you want to specify which file group the target database objects in, set <c>false</c>; otherwise, <c>true</c> (default).
        /// </summary>
        /// <value>Gets or sets a value indicating whether to specify the file group. </value>
        /// <remarks></remarks>
        public bool NoFileGroup { get; set; }

        /// <summary>
        /// If you want to specify which collation the target database objects use, set <c>false</c>; otherwise, <c>true</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to specify the collation. </value>
        /// <remarks></remarks>
        public bool NoCollation { get; set; }

        /// <summary>
        /// If you want to script [non clustered indexes], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to script [non clustered indexes]. </value>
        /// <remarks></remarks>
        public bool NonClusteredIndexes { get; set; }

        /// <summary>
        /// If you want to script [permissions], set <c>true</c>; otherwise, <c>false</c>.
        /// </summary>
        /// <value>Gets or sets a value indicating whether to script [permissions]. </value>
        /// <remarks></remarks>
        public bool Permissions { get; set; }

        /// <summary>
        /// Gets or sets the Boolean property value that specifies whether the script owner is included in the generated script. 
        /// </summary>
        /// <value>
        /// If <c>true</c> (default), the script includes text identifying the script owner.
        /// If <c>false</c>, the script does not identify the script owner.
        /// </value>
        /// <remarks></remarks>
        public bool ScriptOwner { get; set; }

        /// <summary>
        /// Gets or sets the Boolean property value that specifies whether the script operation generates a Transact-SQL script to remove the referenced component. 
        /// </summary>
        /// <value>
        /// A Boolean value that specifies whether to include drop clauses to remove objects in the generated script.
        /// If <c>true</c>, the script operation generates a Transact-SQL script to remove the referenced component.
        /// If <c>false</c> (default), the script operation generates a Transact-SQL script to create the referenced component.
        /// </value>
        /// <remarks></remarks>
        public bool ScriptDrops { get; set; }

        /// <summary>
        /// Gets or sets the Boolean property value that specifies whether the schemas associated with objects are included in the generated script. 
        /// </summary>
        /// <value>
        /// A Boolean value that specifies whether objects that have no schema are included in the generated script.
        /// If <c>true</c>, objects with no schema are not included in the generated script. Otherwise, <c>false</c> (default).
        /// </value>
        /// <remarks></remarks>
        public bool SchemaQualify { get; set; }

        /// <summary>
        /// Gets or sets the Boolean property value that specifies whether to include the definition for triggers in the generated script. 
        /// </summary>
        /// <value>
        /// A Boolean value that specifies whether to include the definition of triggers in the generated script.
        /// If <c>true</c>, the trigger definitions are included in the script. Otherwise, False (default).
        /// </value>
        /// <remarks></remarks>
        public bool ScriptTriggers { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to script tables.
        /// </summary>
        /// <value>If you want to script tables, set <c>true</c> (default); otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool ScriptTables { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to script views.
        /// </summary>
        /// <value>If you want to script views, set <c>true</c> (default); otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool ScriptViews { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to script stored procedures.
        /// </summary>
        /// <value>If you want to script stored procedures, set <c>true</c> (default); otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool ScriptStoredProcedures { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to script assemblies.
        /// </summary>
        /// <value>If you want to script assemblies, set <c>true</c>; otherwise, <c>false</c> (default).</value>
        /// <remarks></remarks>
        public bool ScriptAssemblies { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether the schema is scripted.
        /// </summary>
        /// <value>Set <c>true</c> (default) if you want to [script schemas]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool ScriptSchemas { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to script users.
        /// </summary>
        /// <value>Set <c>true</c> (default) if you want to [script users]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool ScriptUsers { get; set; }

        /// <summary>
        /// Gets or sets the Boolean property value that specifies whether all declarative referential integrity constraints are included in the generated script.
        /// </summary>
        /// <value>
        /// A Boolean value that specifies whether to script all constraint objects that have enforced declarative referential integrity.
        /// If <c>true</c> (default), all constraint objects that have enforced declarative referential integrity are included in the script. Otherwise, <c>false</c>.
        /// </value>
        /// <remarks></remarks>
        public bool AllConstraints { get; set; }

        /// <summary>
        /// Gets or sets the output directory.
        /// </summary>
        /// <value>If you do not specify the output directory, the base directory will be used as an output directory.</value>
        /// <remarks></remarks>
        public string OutputDir
        {
            get
            {
                if (string.IsNullOrEmpty(_outputDir) == false)
                    return _outputDir;

                if (SingleOutputFile)
                {
                    _outputDir = Path.GetDirectoryName(OutputFilePath);
                    return _outputDir;
                }
                
                _outputDir = AppDomain.CurrentDomain.BaseDirectory;
                return _outputDir;
            }
            set
            {
                _outputDir = value;
            }
        }

        /*
        /// <summary>
        /// Gets or sets a value indicating whether to create <see cref="OutputDir"/> if it is not found.
        /// </summary>
        /// <value><c>true</c> if you want to create output dir if it is not found; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool CreateOutputDirIfNotExist { get; set; }
         * */

        /// <summary>
        /// Gets or sets the the output file path.
        /// </summary>
        /// <value>
        /// If you specify <see cref="OutputFileName"/>, all the database objects are scripted into a single file; 
        /// otherwise each object is scripted into a separate file.
        /// </value>
        /// <remarks></remarks>
        public string OutputFileName { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether the script files are overwrite.
        /// </summary>
        /// <value>Set <c>true</c> if you want to script files to be overwritten; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool Overwrite { get; set; }

        /// <summary>
        /// Use this property if you want to script the very specific tables only.
        /// </summary>
        /// <value>Gets or sets the tables to be scripted.</value>
        /// <remarks></remarks>
        public ITaskItem[] IncludeTables { get; set; }

        /// <summary>
        /// Use this property if you want to script all of the table in the target database except the specific tables.
        /// </summary>
        /// <value>Gets or sets the tables not to be scripted.</value>
        /// <remarks></remarks>
        public ITaskItem[] ExcludeTables { get; set; }

        /*
        public bool CopyAllObjects { get; set; }        
        public bool CopyAllDatabaseTriggers { get; set; }         
        public bool CopyAllSchemas { get; set; }
         * * */

        /// <summary>
        /// Gets or sets a value indicating whether to script table records.
        /// </summary>
        /// <value><c>true</c> if you want to script table records; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool CopyData { get; set; }

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

        /// <summary>
        /// Initializes a new instance of the <see cref="GenerateSqlScripts"/> class.
        /// </summary>
        /// <remarks></remarks>
        public GenerateSqlScripts()
        {
            this.AllConstraints = true;
            this.ScriptOwner = true;
            this.ScriptSchemas = true;
            this.NoFileGroup = true;
            this.ScriptTables = true;
            this.ScriptViews = true;
            this.ScriptStoredProcedures = true;
            this.ScriptAssemblies = false;
            this.ScriptUsers = true;
        }

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {            
            if (IsIncludeTablesSpecified && IsExcludeTablesSpecified)
            {
                Log.LogError("An option 'IncludeTables' can not be used with an option 'ExcludeTables'.");
                return false;
            }

            if (ScriptSchemas == false && CopyData == false)
            {
                Log.LogError("Either of 'ScriptSchema' or 'CopyData' should be 'true'.");
                return false;
            }

            return true;
        }

        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            if (Directory.Exists(OutputDir) == false)
                Directory.CreateDirectory(OutputDir);

            ServerConnection serverConnection = GetServerConnection();
            Server server = new Server(serverConnection);
            Database database = server.Databases[Database];
            if (database == null)
            {
                Log.LogError(
                    string.Format("Database '{0}' Not Found!", Database)
                    );
                return false;
            }


            Scripter scripter = new Scripter(server);
            scripter.Options = Options;

  
            IEnumerable tables = GetTables(database.Tables);
            if (ScriptTables == true && ScriptSmoCollectionBase(scripter, tables) == false)
                return false;

            if (ScriptViews == true && ScriptSmoCollectionBase(scripter, database.Views) == false)
                return false;

            if (ScriptStoredProcedures == true && ScriptSmoCollectionBase(scripter, database.StoredProcedures) == false)
                return false;

            if (ScriptAssemblies == true && ScriptSmoCollectionBase(scripter, database.Assemblies) == false)
                return false;

            // if (ScriptSchemas == true && ScriptSmoCollectionBase(scripter, database.Schemas) == false)
                // return false;

            if (ScriptUsers == true && ScriptSmoCollectionBase(scripter, database.Users) == false)
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
                options.ScriptOwner = this.ScriptOwner;
                options.ScriptDrops = this.ScriptDrops;
                options.SchemaQualify = this.SchemaQualify;
                options.ScriptSchema = this.ScriptSchemas;
                options.Triggers = this.ScriptTriggers;
                
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

            return true;
        }

        internal string OutputFilePath
        {
            get { return Path.Combine(OutputDir, OutputFileName); }
        }

        //private bool ScriptSmoCollectionBase(Scripter scripter, IEnumerable<ScriptNameObjectBase> collection)
        //{
        //    return ScriptSmoCollectionBase(scripter, collection as IEnumerable);
        //}

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
                Trace.Assert(dataScripts != null);
                
                scripter.Options.ScriptData = false;
                return;
            }

            if (this.ScriptSchemas == false)
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
