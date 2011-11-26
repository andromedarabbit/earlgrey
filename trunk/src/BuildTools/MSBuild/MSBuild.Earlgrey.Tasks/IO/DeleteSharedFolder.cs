using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO
{
    /// <summary>
    /// Delete the target shared folder.
    /// </summary>
    /// <example>
    /// <code title="Create a shared folder and delete it." lang="xml" source=".\Samples\msbuild-share-and-delete-the-shared-folder.xml" />
    /// </example>
    /// <remarks></remarks>
    public class DeleteSharedFolder : AbstractTask
    {
        /// <inheritdoc />
        public DeleteSharedFolder()
        {
            TreatAsErrorWhenNotExist = true;
            DeleteLocalFolder = false;
        }

        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            return Delete();
        }

        /// <summary>
        /// Deletes the target shared folder.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
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

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            return true;
        }

        /// <summary>
        /// [Required] The target shared folder.
        /// </summary>
        /// <value>Gets or sets the name.</value>
        /// <remarks></remarks>
        [Required]
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to treat as error when the target shared folder named <see cref="Name"/> is not found.
        /// </summary>
        /// <value><c>true</c> if [treat as error when not exist]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool TreatAsErrorWhenNotExist { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether to delete the local folder itself.
        /// </summary>
        /// <value><c>true</c> if [delete local folder]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        public bool DeleteLocalFolder { get; set; }
    }
}
