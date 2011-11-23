using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Ionic.Zip;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.Zip
{
    /// <summary>
    /// Decompress a zip file.
    /// </summary>
    /// <remarks>Implemented by using  http://dotnetzip.codeplex.com</remarks>
    public class Unpack : AbstractTask, IUnpack
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <example>
        /// <code lang="xml">
        /// <![CDATA[
        /// <Zip.Unpack 
        ///     ZipFilePath="$(TempDir)\ZipAFolder.zip" 
		///     TargetDir="$(TempDir)\DstDir" 
		///     Overwrite="true"
	    /// />
        /// ]]>
        /// </code>
        /// </example>
        /// <remarks></remarks>
        public Unpack()
        {
            Overwrite = false;
        }

        /// <summary>
        /// Validates the parameters.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            if (TaskItemHelper.FileExists(ZipFilePath) == false)
            {
                Log.LogError(
                    string.Format("Source file path '{0}' not found!", ZipFilePath.ItemSpec)
                    );
                return false;
            }

            if (TaskItemHelper.FolderExists(TargetDir) == true && Overwrite == false)
            {
                Log.LogError(
                    string.Format("Target directory path '{0}' already exists!", TargetDir.ItemSpec)
                    );
                return false;
            }
            
            return true;
        }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        protected override bool ExecuteCommand()
        {
            using(ZipFile zip = ZipFile.Read(InternalZipFilePath))
            {
                if(string.IsNullOrEmpty(Password) == false)
                {
                    zip.Password = Password;
                }
                zip.ExtractAll(InternalTargetDir, OverwritePolicy);
            }

            return true;
        }

        /// <inheritdoc />
        [Required]
        public ITaskItem TargetDir { get; set; }

        /// <summary>
        /// Gets the internal target dir.
        /// </summary>
        /// <remarks></remarks>
        private string InternalTargetDir
        {
            get { return TaskItemHelper.GetFullPath(TargetDir); }
        }

        /// <inheritdoc />
        public virtual bool Overwrite { get; set; }

        /// <summary>
        /// Gets the overwrite policy.
        /// </summary>
        /// <remarks></remarks>
        private ExtractExistingFileAction OverwritePolicy
        {
            get
            {
                if (Overwrite)
                    return ExtractExistingFileAction.OverwriteSilently;
                return ExtractExistingFileAction.Throw;
            }
        }

        /// <inheritdoc />
        public virtual string Password { get; set; }

        // TODO: 중복 코드 제거하기 (Pack 확인)
        /// <inheritdoc />
        [Required]
        public virtual ITaskItem ZipFilePath { get; set; }

        /// <summary>
        /// Gets the internal zip file path.
        /// </summary>
        /// <remarks></remarks>
        private string InternalZipFilePath
        {
            get { return TaskItemHelper.GetFullPath(ZipFilePath); }
        }
    }
}
