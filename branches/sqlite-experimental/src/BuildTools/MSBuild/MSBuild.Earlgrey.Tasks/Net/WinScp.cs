using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Net
{
    public class WinScp : AbstractWinScp
    {
        public string ScriptText { get; set; }
        public string ScriptFile { get; set; }

        protected override bool ValidateParameters()
        {
            if(base.ValidateParameters() == false)
                return false;

            if(string.IsNullOrEmpty(ScriptFile) == false && string.IsNullOrEmpty(ScriptText) == false)
            {
                Log.LogError("You can't set both of 'ScriptFile' and 'ScriptText' at the same time!");
                return false;
            }

            if (string.IsNullOrEmpty(ScriptFile) == true && string.IsNullOrEmpty(ScriptText) == true)
            {
                Log.LogError("At least one of 'ScriptFile' and 'ScriptText' should be set!");
                return false;
            }

            return base.ValidateParameters();
        }

        protected override string Scripts
        {
            get
            {
                if (string.IsNullOrEmpty(ScriptFile) == false)
                {
                    return File.ReadAllText(ScriptFile);
                }
                return ScriptText;
            }
        }
        
    }
}
