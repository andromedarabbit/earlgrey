using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class ProjectFileNameConverter : IProjectFileNameConverter
    {
        private const string Suffix = "-UnityBuild";
        private static readonly string[] ExtensionsAllowed;

        static ProjectFileNameConverter()
        {
            ExtensionsAllowed = new string[] {".vcproj", ".sln"};
        }

        public virtual bool IsNewName(string name)
        {
            if (string.IsNullOrEmpty(name))
                throw new ArgumentException();

            string extension = Path.GetExtension(name);
            if (string.IsNullOrEmpty(extension))
                throw new ArgumentException();

            if (ExtensionsAllowed.Contains(extension, StringComparer.CurrentCultureIgnoreCase) == false)
                return false;

            string fileNamePart = Path.GetFileNameWithoutExtension(name);
            return fileNamePart.EndsWith(Suffix, StringComparison.CurrentCultureIgnoreCase);
        }

        public virtual string GetNewName(string name)
        {
            if (name == null)
                throw new ArgumentNullException();

            if (name.Length == 0)
                throw new ArgumentException();

            // return name + Suffix;
            string directoryPart = Path.GetDirectoryName(name);
            string fileNamePart = Path.GetFileNameWithoutExtension(name);
            string fileExtensionPart = Path.GetExtension(name);

            string newFileNamePart = fileNamePart + Suffix;

            string newFileName = newFileNamePart + fileExtensionPart;

            return Path.Combine(directoryPart, newFileName);
        }

        public virtual string GetOldName(string name)
        {
            if (name.EndsWith(Suffix) == false)
                throw new ArgumentException();

            return name + Suffix;
        }
    }
}