using System;
using System.Collections.Generic;
using System.Text;

namespace MSBuild.Earlgrey.Tasks
{
    /// <summary>
    /// Represents a MSBuild task using two different steps, validating parameters and execute the task. 
    /// </summary>
    /// <remarks></remarks>
    public abstract class AbstractTask : Microsoft.Build.Utilities.Task
    {
        /// <summary>
        /// When overridden in a derived class, executes the task.
        /// </summary>
        /// <returns>true if the task successfully executed; otherwise, false.</returns>
        /// <remarks></remarks>
        public override bool Execute()
        {
            if(ValidateParameters() == false)
                return false;

            return ExecuteCommand();
        }

        /// <summary>
        /// Validates the parameters.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        protected abstract bool ValidateParameters();

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        protected abstract bool ExecuteCommand();

    }
}
