using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.SevenZip
{
    /// <summary>
    /// Decompress a zip file by using 7-zip.
    /// </summary>
    /// <example>
    /// <code lang="xml">
    /// <![CDATA[
    /// <SevenZip.Unpack 
	/// 	ZipFilePath="$(TempDir)\ZipAFolder.zip" 
	/// 	TargetDir="$(TempDir)\DstDir" 
	/// 	Overwrite="true"
	/// />
    /// ]]>
    /// </code>
    /// </example>
    /// <remarks></remarks>
    public class Unpack : AbstractToolTask, IUnpack
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <remarks></remarks>
        public Unpack()
        {
            Overwrite = false;
        }

        /// <summary>
        /// Indicates whether all task paratmeters are valid.
        /// </summary>
        /// <returns>true if all task parameters are valid; otherwise, false.</returns>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            if (base.ValidateParameters() == false)
                return false;

            return true;
        }

        /// <summary>
        /// Returns a string value containing the command line arguments to pass directly to the executable file.
        /// </summary>
        /// <returns>A string value containing the command line arguments to pass directly to the executable file.</returns>
        /// <remarks></remarks>
        protected override string GenerateCommandLineCommands()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            builder.AppendSwitch("x");

            builder.AppendFileNameIfNotNull(InternalZipFilePath);
            builder.AppendSwitch("-o\"" + InternalTargetDir + "\"");
            // builder.AppendFileNameIfNotNull(InternalTargetDir);

            builder.AppendSwitch(OverwriteSwitch);

            // 7z a archive.7z -psecret
            if (string.IsNullOrEmpty(Password) == false)
            {
                builder.AppendSwitch("-p" + Password);
            }

            return builder.ToString();
        }

        /// <summary>
        /// Gets the name of the executable file to run.
        /// </summary>
        /// <returns>
        /// The name of the executable file to run.
        ///   </returns>
        /// <remarks></remarks>
        protected override string ToolName
        {
            get { return @"7za.exe"; }
        }

        /// <summary>
        /// Gets the internal target dir.
        /// </summary>
        /// <remarks></remarks>
        private string InternalTargetDir
        {
            get { return TaskItemHelper.GetFullPath(TargetDir); }
        }

        /// <inheritdoc />
        [Required]
        public virtual ITaskItem TargetDir { get; set; }

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
                    return ExtractExistingFileAction.OverwriteAll;
                return ExtractExistingFileAction.Skip;
            }
        }

        /// <summary>
        /// Gets the overwrite switch.
        /// </summary>
        /// <remarks></remarks>
        private string OverwriteSwitch
        {
            get
            {
                if (OverwritePolicy == ExtractExistingFileAction.OverwriteAll)
                    return "-aoa";
                if (OverwritePolicy == ExtractExistingFileAction.Skip)
                    return "-aos";
                if (OverwritePolicy == ExtractExistingFileAction.AutoRenameExtractingFile)
                    return "-aou";
                if (OverwritePolicy == ExtractExistingFileAction.AutoRenameExistingFile)
                    return "-aot";

                throw new Exception();
            }
        }

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


        /// <inheritdoc />
        public virtual string Password { get; set; }
    }

}
