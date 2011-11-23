using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Ionic.Zip;
using Ionic.Zlib;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.Zip
{

    /// <summary>
    /// Compress a folder and its sub-folders and sub-files.
    /// </summary>
    /// <example>
    /// <code lang="xml">
    /// <![CDATA[
    /// <Zip.Pack 
	/// 	SrcFolder="$(RootDir)\Data" 
	/// 	ZipFilePath="$(TempDir)\ZipAFolder.zip" 
    ///		ContainsRootDir="false"
    /// />
    /// ]]>
    /// </code>
    /// </example>
    /// <remarks>Implemented by using  http://dotnetzip.codeplex.com</remarks>
    public class Pack : AbstractTask, IPack
    {
        private readonly ZipFile _zip;
        private string[] _excludes;
        private readonly List<Regex> _excludePatterns;

        /// <summary>
        /// Initializes a new instance of the <see cref="T:System.Object"/> class.
        /// </summary>
        /// <remarks></remarks>
        public Pack()
        {
            _zip = new ZipFile();
            _excludes = null;
            _excludePatterns = new List<Regex>();

            Overwrite = false;
            ContainsRootDir = true;
            ZipLevel = FromCompressionLevel(CompressionLevel.Default);
        }

        /// <summary>
        /// Validates the parameters.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            if (TaskItemHelper.FolderExists(SrcFolder) == false)
            {
                Log.LogError(
                    string.Format("Source folder path '{0}' not found!", SrcFolder.ItemSpec)
                    );
                return false;
            }

            if (TaskItemHelper.FileExists(ZipFilePath) == true && Overwrite == false)
            {
                Log.LogError(
                    string.Format("Destination file path '{0}' already exists!", ZipFilePath.ItemSpec)
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
            // Pack
            _zip.CompressionLevel = InternalZipLevel;

            if (string.IsNullOrEmpty(Password) == false)
            {
                _zip.Encryption = EncryptionAlgorithm.WinZipAes256;
                _zip.Password = Password;
            }

            string directoryPathInArchive = string.Empty;
            if(ContainsRootDir)
                directoryPathInArchive = Path.GetFileName(InternalSrcFolder);
            
            _zip.AddDirectory(InternalSrcFolder, directoryPathInArchive);

            if (HasExcludePattern())
            {
                var entriesToBeRemoved = _zip.Entries.Where(IsToBeExcluded).ToList();
                _zip.RemoveEntries(entriesToBeRemoved);
            }

            string zipFileDir = Directory.GetParent(InternalZipFilePath).FullName;
            if (Directory.Exists(zipFileDir) == false)
                Directory.CreateDirectory(zipFileDir);

            _zip.Save(InternalZipFilePath);
            

            return true;
        }

        /// <summary>
        /// Determines whether [has exclude pattern].
        /// </summary>
        /// <returns><c>true</c> if [has exclude pattern]; otherwise, <c>false</c>.</returns>
        /// <remarks></remarks>
        private bool HasExcludePattern()
        {
            return Excludes != null && Excludes.Length > 0;
        }

        /// <summary>
        /// Gets the internal SRC folder.
        /// </summary>
        /// <remarks></remarks>
        private string InternalSrcFolder
        {
            get { return TaskItemHelper.GetFullPath(SrcFolder); }
        }

        /// <summary>
        /// Gets the internal zip file path.
        /// </summary>
        /// <remarks></remarks>
        private string InternalZipFilePath
        {
            get { return TaskItemHelper.GetFullPath(ZipFilePath); }
        }

        /// <inheritdoc />
        [Required]
        public virtual ITaskItem SrcFolder { get; set; }

        /// <inheritdoc />
        public virtual string[] Excludes
        {
            get { return _excludes; }
            set
            {
                _excludes = value;

                if(value == null)                
                    return;                

                foreach (string pattern in value)
                {
                    Regex regex = new Regex(pattern);
                    _excludePatterns.Add(regex);
                }

                
            }
        }

        /// <inheritdoc />
        [Required]
        public virtual ITaskItem ZipFilePath { get; set; }

        /// <inheritdoc />
        public virtual bool Overwrite { get; set; }

        /// <inheritdoc />
        public virtual int ZipLevel { get; set; }

        /// <summary>
        /// Gets the internal zip level.
        /// </summary>
        /// <remarks></remarks>
        private CompressionLevel InternalZipLevel
        {
            get { return ToCompressionLevel(ZipLevel); }
        }

        /// <summary>
        /// From the compression level.
        /// </summary>
        /// <param name="level">The level.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private static int FromCompressionLevel(CompressionLevel level)
        {
            return (int) level;
        }

        /// <summary>
        /// Toes the compression level.
        /// </summary>
        /// <param name="level">The level.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private static CompressionLevel ToCompressionLevel(int level)
        {
            return (CompressionLevel) level;
        }

        /// <inheritdoc />
        public virtual string Password { get; set; }

        /// <inheritdoc />
        public virtual bool ContainsRootDir { get; set; }

        /// <summary>
        /// Determines whether [is to be excluded] [the specified entry].
        /// </summary>
        /// <param name="entry">The entry.</param>
        /// <returns><c>true</c> if [is to be excluded] [the specified entry]; otherwise, <c>false</c>.</returns>
        /// <remarks></remarks>
        private bool IsToBeExcluded(ZipEntry entry)
        {
            string fileName = entry.FileName;
            return _excludePatterns.Any(regex => regex.IsMatch(fileName) == true);
        }
    }
}
