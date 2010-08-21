using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using IniParser;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{
    public abstract class AbstractIniFile : AbstractTask
    {
        private readonly FileIniDataParser _parser;
        private IniData _parsedData;

        protected AbstractIniFile()
        {
            _parser = new FileIniDataParser();
        }

        protected override bool ExecuteCommand()
        {
            _parsedData = _parser.LoadFile(FullPath);
         
            return true;
        }

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

        protected FileIniDataParser Parser
        {
            get
            {
                Debug.Assert(_parser != null);
                return _parser;
            }
        }

        protected IniData Data
        {
            get
            {
                Debug.Assert(_parsedData != null); 
                return _parsedData;
            }
        }

        [Required]
        public ITaskItem FilePath { get; set; }

        protected string FullPath
        {
            get { return FilePath.GetMetadata("FullPath"); }
        }
    }
}
