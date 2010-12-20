using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class ProjectConfigurationNameConverter : AbstractProjectConfigurationNameConverter
    {
        private const string Suffix = "-UnityBuild";

        public override bool IsNewName(string configurationName)
        {
            return configurationName.EndsWith(Suffix, StringComparison.CurrentCultureIgnoreCase);
        }

        public override string GetNewName(string name)
        {
            if (name == null)
                throw new ArgumentNullException();

            if (name.Length == 0)
                throw new ArgumentException();

            return name + Suffix;
        }

        public override string GetOldName(string name)
        {
            if (name.EndsWith(Suffix) == false)
                throw new ArgumentException();

            return name + Suffix;
        }
    }
}
