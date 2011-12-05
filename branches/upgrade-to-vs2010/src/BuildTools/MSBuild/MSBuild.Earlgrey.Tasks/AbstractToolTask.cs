using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks
{
    /// <summary>
    /// Represents a MSBuild task using an external tool. This provides more convenient and useful file search algorithm.
    /// </summary>
    /// <remarks></remarks>
    public abstract class AbstractToolTask : ToolTask
    {
        /// <inheritdoc />
        protected override string GenerateFullPathToTool()
        {
            return ToolsSearch.FindExternalTool(ToolName);
        }
    }
}
