using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using IniParser;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{

    /// <summary>
    /// A class which provides basic functionalities to manipulate an .ini file.
    /// </summary>
    /// <remarks></remarks>
    public abstract class AbstractIniFile : AbstractTask
    {
        private readonly AdvancedFileIniDataParser _parser;
        private IniData _parsedData;

        /// <inheritdoc />
        protected AbstractIniFile()
        {
            _parser = new AdvancedFileIniDataParser();
        }

        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            _parsedData = _parser.LoadFile(FullPath);
         
            return true;
        }

        /// <inheritdoc/>
        protected override bool ValidateParameters()
        {
            if(File.Exists(FullPath) == false)
            {
                Log.LogError( 
                    string.Format("Ini file not found at '{0}'", FilePath)
                    );
                return false;
            }

            return true;
        }

        /// <summary>
        /// Gets the parser for .ini file.
        /// </summary>
        /// <remarks></remarks>
        protected FileIniDataParser Parser
        {
            get
            {
                Trace.Assert(_parser != null);
                return _parser;
            }
        }

        /// <summary>
        /// Gets the data representing .ini file.
        /// </summary>
        /// <remarks></remarks>
        protected IniData Data
        {
            get
            {
                Trace.Assert(_parsedData != null); 
                return _parsedData;
            }
        }

        /// <summary>
        /// Gets the name of the global section.
        /// </summary>
        /// <remarks></remarks>
        protected string GlobalSectionName
        {
            get { return "[]"; }
        }

        /// <summary>
        /// [Required] Gets or sets the target .ini file path.
        /// </summary>
        /// <value>The file path.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem FilePath { get; set; }

        /// <summary>
        /// Gets the full path of the target .ini file.
        /// </summary>
        /// <remarks></remarks>
        protected string FullPath
        {
            get { return FilePath.GetMetadata("FullPath"); }
        }
    }
}
