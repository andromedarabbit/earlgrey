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
            Debug.Assert(string.IsNullOrEmpty(name) == false);

            string extension = Path.GetExtension(name);
            if(string.IsNullOrEmpty(extension))
                throw new ArgumentException();

            // if(ExtensionsAllowed.Contains())


            // return name.EndsWith(Suffix, StringComparison.CurrentCultureIgnoreCase);
            throw new NotImplementedException();
        }

        public virtual string GetNewName(string name)
        {
            if (name == null)
                throw new ArgumentNullException();

            if (name.Length == 0)
                throw new ArgumentException();

            return name + Suffix;
        }

        public virtual string GetOldName(string name)
        {
            if (name.EndsWith(Suffix) == false)
                throw new ArgumentException();

            return name + Suffix;
        }
    }
}
