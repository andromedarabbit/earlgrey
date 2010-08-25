using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Ionic.Zip;
using Ionic.Zlib;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Zip
{
    /// <summary>
    /// Zip 파일로 폴더를 압축한다.
    /// </summary>
    /// <remarks>아직 개별 파일 압축은 지원하지 않는다.</remarks>
    /// <see cref="http://dotnetzip.codeplex.com"/>
    public class Pack : AbstractTask
    {
        private readonly ZipFile _zip;
        private string[] _excludes;
        private readonly List<Regex> _excludePatterns;

        public Pack()
        {
            _zip = new ZipFile();
            _excludes = null;
            _excludePatterns = new List<Regex>();

            Overwrite = false;
            ContainsRootDir = true;
            ZipLevel = FromCompressionLevel(CompressionLevel.Default);
        }

        protected override bool ValidateParameters()
        {
            if (Directory.Exists(InternalSrcFolder) == false)
            {
                Log.LogError(
                    string.Format("Source folder path '{0}' not found!", SrcFolder.ItemSpec)
                    );
                return false;
            }

            if (File.Exists(InternalZipFilePath) == true && Overwrite == false)
            {
                Log.LogError(
                    string.Format("Destination file path '{0}' already exists!", ZipFilePath.ItemSpec)
                    );
                return false;
            }

            return true;
        }

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

        private bool HasExcludePattern()
        {
            return Excludes != null && Excludes.Length > 0;
        }

        [Required]
        public ITaskItem SrcFolder { get; set; }

        private static string GetFullPath(ITaskItem taskItem)
        {
            return taskItem.GetMetadata("FullPath");
        }

        private string InternalSrcFolder
        {
            get { return GetFullPath(SrcFolder); }
        }

        public string[] Excludes
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

        private bool IsToBeExcluded(ZipEntry entry)
        {
            string fileName = entry.FileName;
            return _excludePatterns.Any(regex => regex.IsMatch(fileName) == true);
        }

        [Required]
        public ITaskItem ZipFilePath { get; set; }

        private string InternalZipFilePath
        {
            get { return GetFullPath(ZipFilePath); }
        }

        public bool Overwrite { get; set; }

        public int ZipLevel { get; set; }

        private CompressionLevel InternalZipLevel
        {
            get { return ToCompressionLevel(ZipLevel); }
        }

        private static int FromCompressionLevel(CompressionLevel level)
        {
            return (int) level;
        }

        private static CompressionLevel ToCompressionLevel(int level)
        {
            return (CompressionLevel) level;
        }

        public string Password { get; set; }

        public bool ContainsRootDir { get; set; }
    }
}
