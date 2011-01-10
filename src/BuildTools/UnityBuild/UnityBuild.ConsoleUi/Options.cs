using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CommandLine;

namespace UnityBuild.ConsoleUi
{
    /*
     * Future features
     * - Source code encoding
     * - Copy a solution and project files 
     * */
    sealed class Options
    {
        private const string InputFileSwitch = "i";
        private const string InputFileLongSwitch = "input";
        private const string InputFileHelp = "Solution file path.";
        private const bool InputFileRequired = true;

        [Option(InputFileSwitch, InputFileLongSwitch, Required = InputFileRequired, HelpText = InputFileHelp)]
        public string InputFile;


        private const string ExcludedProjectsSwitch = "e";
        private const string ExcludedProjectsLongSwitch = "exclude";
        private const char ExcludedProjectsSeparator = ';';
        private const string ExcludedProjectsSeparatorString = ";";
        private const string ExcludedProjectsHelp
            = "Visual C++ project names you want to exclude. Separated by a ';'."
            + " For instance, -" + ExcludedProjectsSwitch + "\"Earlgrey\"" + ExcludedProjectsSeparatorString + "\"Earlgrey.Test\""
            ;
        private const bool ExcludedProjectsRequired = false;


        [OptionList(ExcludedProjectsSwitch, ExcludedProjectsLongSwitch, Required = ExcludedProjectsRequired, HelpText = ExcludedProjectsHelp, Separator = ExcludedProjectsSeparator)]
        public IList<string> ExcludedProjects;


        private const string VerboseSwitch = "v";
        private const string VerboseLongSwitch = "verbose";
        private const string VerboseHelp = "Print details during execution.";
        private const bool VerboseRequired = false;

        [Option(VerboseSwitch, VerboseLongSwitch, Required = VerboseRequired, HelpText = VerboseHelp)]
        public bool Verbose;


        private const string CopySolutionSwitch = "c";
        private const string CopySolutionLongSwitch = "copy";
        private const string CopySolutionHelp = "Copy the solution/projects and use them.";
        private const bool CopySolutionRequired = false;

        [Option(CopySolutionSwitch, CopySolutionLongSwitch, Required = CopySolutionRequired, HelpText = CopySolutionHelp)]
        public bool CopySolution;


        private const string OptimizationSwitch = "o";
        private const string OptimizationLongSwitch = "optimization";
        private const string OptimizationHelp = "Possible values are 'Normal' and 'Best'.";
        private const bool OptimizationRequired = false;

        [Option(OptimizationSwitch, OptimizationLongSwitch, Required = OptimizationRequired, HelpText = OptimizationHelp)]
        public OptimizationLevel Optimization;

        private const string MaxFilesPerFileSwitch = "m";
        private const string MaxFilesPerFileLongSwitch = "MaxFiles";
        private const string MaxFilesPerFileHelp = "The maximum number of source files included in a new .cpp file for UnityBuild.";
        private const bool MaxFilesPerFileRequired = false;

        [Option(MaxFilesPerFileSwitch, MaxFilesPerFileLongSwitch, Required = MaxFilesPerFileRequired, HelpText = MaxFilesPerFileHelp)]
        public int MaxFilesPerFile;


        private const string RemoveUnityBuildSwitch = "r";
        private const string RemoveUnityBuildLongSwitch = "Remove";
        private const string RemoveUnityBuildHelp = "Remove 'UnityBuild' configurations from the solution and its projects.";
        private const bool RemoveUnityBuildRequired = false;

        [Option(RemoveUnityBuildSwitch, RemoveUnityBuildLongSwitch, Required = RemoveUnityBuildRequired, HelpText = RemoveUnityBuildHelp)]
        public bool RemoveUnityBuild;


        public Options()
        {
            InputFile = null;
            ExcludedProjects = new List<string>();
            CopySolution = false;
            Verbose = false;
            Optimization = OptimizationLevel.Normal;
            MaxFilesPerFile = 0;
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

            usage.Append(" -" + CopySolutionSwitch + ", --" + CopySolutionLongSwitch + " ");
            usage.AppendLine(GetRquiredString(CopySolutionRequired));
            usage.AppendLine("   " + CopySolutionHelp);

            usage.Append(" -" + OptimizationSwitch + ", --" + OptimizationLongSwitch + " ");
            usage.AppendLine(GetRquiredString(OptimizationRequired));
            usage.AppendLine("   " + OptimizationHelp);

            usage.Append(" -" + MaxFilesPerFileSwitch + ", --" + MaxFilesPerFileLongSwitch + " ");
            usage.AppendLine(GetRquiredString(MaxFilesPerFileRequired));
            usage.AppendLine("   " + MaxFilesPerFileHelp);

            usage.Append(" -" + RemoveUnityBuildSwitch + ", --" + RemoveUnityBuildLongSwitch + " ");
            usage.AppendLine(GetRquiredString(RemoveUnityBuildRequired));
            usage.AppendLine("   " + RemoveUnityBuildHelp);

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
            summary.Append(" * Input file path is \"" + InputFile + "\"");

            if(RemoveUnityBuild == true)
            {
                summary.AppendLine(
                    "Remove option is turned on. Other switches except -e that might be specified will be ignored!"
                    );

            }

            if (CopySolution == true)
            {
                summary.AppendLine(", its copied version will be converted and used");
            }
            summary.AppendLine(".");

            if (HasExcludedProjects())
            {
                summary.Append(" * Following projects are excluded : ");
                foreach (string project in ExcludedProjects)
                {
                    summary.Append("\"" + project + "\"");
                }
                summary.AppendLine();
            }

            if (MaxFilesPerFile > 0)
                summary.AppendLine(" * Maximum number of source files included in a new .cpp file is " + MaxFilesPerFile + ".");

            if (Verbose)
                summary.AppendLine(" * Verbose mode is turned on.");
            else
                summary.AppendLine(" * Verbose mode is turned off.");

            summary.AppendLine(string.Format(" * Current optimization level is '{0}'.", Optimization));

            return summary.ToString();
        }

        public override string ToString()
        {
            return GetSummary();
        }

        public BuilderOptions GetBuilderOptions()
        {
            BuilderOptions builderOptions = new BuilderOptions();
            builderOptions.CopySolution = this.CopySolution;
            builderOptions.GroupByFilter = this.NeedToGroupByFilter;
            builderOptions.MaxFilesPerFile = this.MaxFilesPerFile;

            if (this.HasExcludedProjects())
                builderOptions.ExcludeProjects(this.ExcludedProjects);

            return builderOptions;
        }

        private bool NeedToGroupByFilter
        {
            get
            {
                if (Optimization == OptimizationLevel.Best)
                    return false;

                return true;
            }
        }
    }
}
