using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public abstract class AbstractConfigurationNameConverter
    {
        public abstract string GetNewName(string configurationName);

        public static string GetNewName(string configurationPlatformName, AbstractConfigurationNameConverter converter)
        {
            Debug.Assert(string.IsNullOrEmpty(configurationPlatformName) == false);
            Debug.Assert(converter != null);

            string[] tokens = configurationPlatformName.Split(new char[] { '|' }, 2, StringSplitOptions.RemoveEmptyEntries);
            Debug.Assert(tokens.Length == 2);

            string projectConfigurationName = tokens[0];
            string projectPlatformName = tokens[1];
            string projectProperty = string.Empty;
            Debug.Assert(string.IsNullOrEmpty(projectConfigurationName) == false);
            Debug.Assert(string.IsNullOrEmpty(projectPlatformName) == false);

            if (projectPlatformName.Contains("."))
            {
                Debug.Assert(projectPlatformName.Count(c => c == '.') < 3);

                string[] platformNameAndProperty = projectPlatformName.Split(new char[] { '.' }, 2, StringSplitOptions.RemoveEmptyEntries);
                projectPlatformName = platformNameAndProperty[0];
                projectProperty = platformNameAndProperty[1];

                Debug.Assert(string.IsNullOrEmpty(projectConfigurationName) == false);
                Debug.Assert(string.IsNullOrEmpty(projectPlatformName) == false);
            }


            string newProjectConfigurationName = converter.GetNewName(projectConfigurationName);
            Debug.Assert(string.IsNullOrEmpty(newProjectConfigurationName) == false);

            string newName = newProjectConfigurationName + "|" + projectPlatformName;
            if (projectProperty.Length > 0)
            {
                newName = newName + "." + projectProperty;
            }
            return newName;
        }

    }
}
