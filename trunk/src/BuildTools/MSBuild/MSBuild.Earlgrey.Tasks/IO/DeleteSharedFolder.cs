using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO
{
    public class DeleteSharedFolder : AbstractTask
    {
        public DeleteSharedFolder()
        {
            TreatAsErrorWhenNotExist = true;
            DeleteLocalFolder = false;
        }

        protected override bool ExecuteCommand()
        {
            return Delete();
        }

        private bool Delete()
        {
            using (Win32Share tempShare = Win32Share.GetNamedShare(Name))
            {
                if (tempShare == null)
                {
                    string msg = string.Format("Shared folder with a name '{0}' not found!", Name);

                    if (TreatAsErrorWhenNotExist)
                    {
                        Log.LogError(msg);
                        return false;
                    }

                    Log.LogMessage(msg);
                    return true;
                }


                Win32Share.MethodStatus deleteResult = tempShare.Delete();
                if (deleteResult != Win32Share.MethodStatus.Success)
                {
                    Log.LogError("Couldn't delete the shared folder!");
                    return false;
                }

                if (DeleteLocalFolder)
                    Directory.Delete(tempShare.Path, true);

                return true;
            }
        }

        protected override bool ValidateParameters()
        {
            return true;
        }
        
        [Required]
        public string Name { get; set; }

        public bool TreatAsErrorWhenNotExist { get; set; }

        public bool DeleteLocalFolder { get; set; }
    }
}
