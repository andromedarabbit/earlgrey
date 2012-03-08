using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class SrcFileAppend : IDisposable
    {
        private readonly List<KeyValuePair<string, PrecompiledHeaderOptions>> _stdafxs;
        private readonly string _dstFilePath;
        private readonly string _projectDir;

        private readonly List<IFileType> _srcFiles;

        private readonly List<string> _buildConfigurations;
        private readonly List<string> _buildConfigurationsExcluded;


        public SrcFileAppend(IEnumerable<KeyValuePair<string, PrecompiledHeaderOptions>> stdafxs, string dstFilePath,
                             string projectDir, IEnumerable<string> buildConfigurations)
            : this(stdafxs, dstFilePath, projectDir, buildConfigurations, new List<string>())
        {
        }

        public SrcFileAppend(
            IEnumerable<KeyValuePair<string, PrecompiledHeaderOptions>> stdafxs, string dstFilePath, string projectDir,
            IEnumerable<string> buildConfigurations, IEnumerable<string> buildConfigurationsExcluded
            )
        {
            Debug.Assert(string.IsNullOrEmpty(dstFilePath) == false);
            Debug.Assert(Directory.Exists(projectDir));

            _stdafxs = new List<KeyValuePair<string, PrecompiledHeaderOptions>>(stdafxs);

            _dstFilePath = dstFilePath;
            _projectDir = projectDir;
            _srcFiles = new List<IFileType>();


            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);
        }

        public string DstFilePath
        {
            get { return _dstFilePath; }
        }

        public string ProjectDir
        {
            get { return _projectDir; }
        }

        public void Open()
        {
            Debug.Assert(Directory.Exists(_projectDir));
        }

        #region IDisposable

        // Track whether Dispose has been called.
        private bool _disposed = false;

        // Implement IDisposable.
        // Do not make this method virtual.
        // A derived class should not be able to override this method.
        public void Dispose()
        {
            Dispose(true);
            // This object will be cleaned up by the Dispose method.
            // Therefore, you should call GC.SupressFinalize to
            // take this object off the finalization queue
            // and prevent finalization code for this object
            // from executing a second time.
            GC.SuppressFinalize(this);
        }

        // Dispose(bool disposing) executes in two distinct scenarios.
        // If disposing equals true, the method has been called directly
        // or indirectly by a user's code. Managed and unmanaged resources
        // can be disposed.
        // If disposing equals false, the method has been called by the
        // runtime from inside the finalizer and you should not reference
        // other objects. Only unmanaged resources can be disposed.
        private void Dispose(bool disposing)
        {
            // Check to see if Dispose has already been called.
            if (!this._disposed)
            {
                // If disposing equals true, dispose all managed
                // and unmanaged resources.
                if (disposing)
                {
                    // Dispose managed resources.
                }

                // Call the appropriate methods to clean up
                // unmanaged resources here.
                // If disposing is false,
                // only the following code is executed.


                // Note disposing has been done.
                _disposed = true;
            }
        }

        #endregion

        public void AddSrcFile(IFileType file)
        {
            Debug.Assert(file != null);
            Debug.Assert(file.IsSrcFile);

            _srcFiles.Add(file);
        }

        public bool Merge()
        {
            if (_srcFiles.Count == 0)
                return false;

            if (File.Exists(_dstFilePath))
                File.Delete(_dstFilePath);

            using (StreamWriter sw = new StreamWriter(_dstFilePath, true, Encoding.Default))
            {
                IEnumerable<string> stdAfxFileNames =
                    _stdafxs
                        .Select(item => item.Value)
                        .Where(
                            item =>
                            item.UsePrecompiledHeader == UsePrecompiledHeaderOptions.Use ||
                            item.UsePrecompiledHeader == UsePrecompiledHeaderOptions.Create)
                        .Select(item => item.PrecompiledHeaderThrough)
                        .Distinct()
                    ;

                Debug.Assert(stdAfxFileNames.Count() < 2);

                if (stdAfxFileNames.Count() == 1)
                {
                    sw.WriteLine("#include \"" + stdAfxFileNames.First() + "\"");
                }

                foreach (FileType srcFile in _srcFiles)
                {
                    WriteInclude(sw, srcFile);
                }
            }

            return true;
        }

        private void WriteInclude(TextWriter sw, FileType srcFile)
        {
            // IEnumerable<string> configurationsExcluded = srcFile.BuildConfigurationsWhenExcludedFromBuild; // Unity
            // build - all
            // ex - not unity

            var configurationsIncluded = _buildConfigurations.Where(
                item => _buildConfigurationsExcluded.Contains(item, StringComparer.CurrentCultureIgnoreCase)
                );

            IEnumerable<string> configurationsExcluded = srcFile.BuildConfigurationsWhenExcludedFromBuild.Where(
                item => configurationsIncluded.Contains(item, StringComparer.CurrentCultureIgnoreCase)
                );


            if (configurationsExcluded.Count() == 0)
            {
                sw.WriteLine("#include \"" + srcFile.FileName + "\"");
                return;
            }

            foreach (string configurationPlatform in _buildConfigurations)
            {
                bool exclude =
                    configurationsExcluded.Contains(configurationPlatform, StringComparer.CurrentCultureIgnoreCase);
                WriteInclude(sw, configurationPlatform, srcFile.FileName, exclude);
            }
        }


        private static void WriteInclude(TextWriter sw, string configurationPlatform, string srcFileName, bool exclude)
        {
            // TODO: 하드코딩, 중복 코드
            string definition = "UNITYBUILD_" + configurationPlatform.ToUpper();
            definition = definition.Replace("|", "_");
            definition = definition.Replace("-", "_");

            sw.WriteLine("#ifdef " + definition);
            if (exclude == false)
                sw.WriteLine("#include \"" + srcFileName + "\"");
            sw.WriteLine("#endif");
        }
    }
}