using System;
using System.Collections.Generic;

using System.Text;

namespace MSBuild.Earlgrey.Tasks
{
    public abstract class AbstractTask : Microsoft.Build.Utilities.Task
    {
        public override bool Execute()
        {
            if(ValidateParameters() == false)
                return false;

            return ExecuteCommand();
        }

        protected abstract bool ValidateParameters();

        protected abstract bool ExecuteCommand();

    }
}
