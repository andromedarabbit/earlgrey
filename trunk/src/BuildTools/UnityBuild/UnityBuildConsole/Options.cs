﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CommandLine;

namespace UnityBuildConsole
{
    sealed class Options
    {
        private const string InputFileSwitch = "i";
        private const string InputFileLongSwitch = "input";
        private const string InputFileHelp = "Solution file path.";
        private const bool InputFileRequired = true;

        private const string ExcludedProjectsSwitch = "e";
        private const string ExcludedProjectsLongSwitch = "exclude";
        private const char ExcludedProjectsSeparator = ';';
        private const string ExcludedProjectsSeparatorString = ";";
        private const string ExcludedProjectsHelp 
            = "Visual C++ project names you want to exclude. Separated by a ';'."
            + " For instance, -" + ExcludedProjectsSwitch + "\"Earlgrey\"" + ExcludedProjectsSeparatorString + "\"Earlgrey.Test\""
            ;
        private const bool ExcludedProjectsRequired = false;
        

        private const string VerboseSwitch = "v";
        private const string VerboseLongSwitch = "verbose";
        private const string VerboseHelp = "Print details during execution.";
        private const bool VerboseRequired = false;

        [Option(InputFileSwitch, InputFileLongSwitch, Required = InputFileRequired, HelpText = InputFileHelp)] 
        public string InputFile;

        [OptionList(ExcludedProjectsSwitch, ExcludedProjectsLongSwitch, Required = ExcludedProjectsRequired, HelpText = ExcludedProjectsHelp, Separator = ExcludedProjectsSeparator)] 
        public IList<string> ExcludedProjects;

        [Option(VerboseSwitch, VerboseLongSwitch, Required = VerboseRequired, HelpText = VerboseHelp)]
        public bool Verbose;

        public Options()
        {
            InputFile = null;
            ExcludedProjects = null;
            Verbose = false;
        }

        [HelpOption(HelpText = "Display this help screen.")]
        public string GetUsage()
        {
            var usage = new StringBuilder();
            usage.AppendLine("How to use!");
            usage.Append(" -" + InputFileSwitch + ", --" + InputFileLongSwitch + " ");
            usage.AppendLine(GetRquiredString(InputFileRequired));                
            usage.AppendLine("   " + InputFileHelp);

            usage.Append(" -" + ExcludedProjectsSwitch + ", --" + ExcludedProjectsLongSwitch + " ");
            usage.AppendLine(GetRquiredString(ExcludedProjectsRequired));                
            usage.AppendLine("   " + ExcludedProjectsHelp);

            usage.Append(" -" + VerboseSwitch + ", --" + VerboseLongSwitch + " ");
            usage.AppendLine(GetRquiredString(VerboseRequired));
            usage.AppendLine("   " + VerboseHelp);

            return usage.ToString();
        }

        private static string GetRquiredString(bool yesOrNo)
        {
            if (yesOrNo)
                return "(Required)";

            return "(Not Required)";
        }

        // TODO: Commandline Parser 라이브러리가 public 프로퍼티를 모두 호출하기 떄문에 함수로 만든다.
        // IgnoreAttribute 같은 게 있어야 할텐데....
        public bool HasExcludedProjects()
        {
            return ExcludedProjects != null 
                && ExcludedProjects.Count() > 0
                ; 
        }

        public string GetSummary()
        {
            StringBuilder summary = new StringBuilder();
            summary.AppendLine("[Switches]");
            summary.AppendLine(" * Input file path is \"" + InputFile + "\".");
            
            if(HasExcludedProjects())
            {
                summary.Append( " * Following projects are excluded : ");
                foreach (string project in ExcludedProjects)
                {
                    summary.Append("\"" + project + "\"");
                }
                summary.AppendLine();
            }

            if (Verbose)
                summary.AppendLine(" * Verbose mode is turned on.");
            else
                summary.AppendLine(" * Verbose mode is turned off.");

            return summary.ToString();
        }
        
        public override string ToString()
        {
            return GetSummary();
        }
    }
}
