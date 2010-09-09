using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks
{
    public abstract class AbstractToolTask : ToolTask
    {
        protected override string GenerateFullPathToTool()
        {
            return ToolsSearch.FindExternalTool(ToolName);
        }
        

    }
}
