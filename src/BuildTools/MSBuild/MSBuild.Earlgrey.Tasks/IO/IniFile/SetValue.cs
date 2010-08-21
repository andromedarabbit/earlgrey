using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using IniParser;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{
    public class SetValue : AbstractIniFile
    {
        public SetValue()
        {
        }

        protected override bool ExecuteCommand()
        {
            if(base.ExecuteCommand() == false)
                return false;

            Data[Section][Key] = Value;

            Parser.SaveFile(FullPath, Data);
            
            return true;
        }

        protected override bool ValidateParameters()
        {
            return base.ValidateParameters();
        }

        [Required]
        public string Section { get; set; }

        [Required]
        public string Key { get; set; }

        [Required]
        public string Value { get; set; }
    }
}
