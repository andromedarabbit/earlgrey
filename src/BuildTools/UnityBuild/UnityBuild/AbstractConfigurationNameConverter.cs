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
        public abstract string GetOldName(string configurationName);
        public abstract bool IsNewName(string configurationName);

        internal static string GetConfigurationPlatform(string configuration, string platform)
        {
            return configuration + "|" + platform;
        }

        internal static string GetConfigurationPlatform(string configuration, string platform, string property)
        {
            if (string.IsNullOrEmpty(property))
                return GetConfigurationPlatform(configuration, platform);

            Debug.Assert(property == "Build.0" || property == "ActiveCfg");

            return GetConfigurationPlatform(configuration, platform) + "." + property;
        }

        internal static string GetNewName(string configurationPlatformName, AbstractConfigurationNameConverter converter)
        {
            Debug.Assert(string.IsNullOrEmpty(configurationPlatformName) == false);
            Debug.Assert(converter != null);

            string projectPlatformName;
            string projectProperty;
            string projectConfigurationName;

            SplitConfigurationPlatform(configurationPlatformName, out projectConfigurationName, out projectPlatformName,
                                       out projectProperty);

            string newProjectConfigurationName = converter.GetNewName(projectConfigurationName);
            Debug.Assert(string.IsNullOrEmpty(newProjectConfigurationName) == false);

            return GetConfigurationPlatform(newProjectConfigurationName, projectPlatformName, projectProperty);
        }

        internal static void SplitConfigurationPlatform(string configurationPlatformName,
                                                        out string projectConfigurationName,
                                                        out string projectPlatformName, out string projectProperty)
        {
            string[] tokens = configurationPlatformName.Split(new char[] {'|'}, 2, StringSplitOptions.RemoveEmptyEntries);
            Debug.Assert(tokens.Length == 2);

            projectConfigurationName = tokens[0];
            projectPlatformName = tokens[1];
            projectProperty = string.Empty;
            Debug.Assert(string.IsNullOrEmpty(projectConfigurationName) == false);
            Debug.Assert(string.IsNullOrEmpty(projectPlatformName) == false);

            if (projectPlatformName.Contains("."))
            {
                Debug.Assert(projectPlatformName.Count(c => c == '.') < 3);

                string[] platformNameAndProperty = projectPlatformName.Split(new char[] {'.'}, 2,
                                                                             StringSplitOptions.RemoveEmptyEntries);
                projectPlatformName = platformNameAndProperty[0];
                projectProperty = platformNameAndProperty[1];

                Debug.Assert(string.IsNullOrEmpty(projectConfigurationName) == false);
                Debug.Assert(string.IsNullOrEmpty(projectPlatformName) == false);
            }
        }

        internal static string GetConfiguration(string configurationPlatform)
        {
            string platformName;
            string configurationName;
            string property;

            SplitConfigurationPlatform(
                configurationPlatform, out configurationName, out platformName, out property
                );

            Debug.Assert(string.IsNullOrEmpty(platformName) == false);
            Debug.Assert(string.IsNullOrEmpty(configurationName) == false);
            Debug.Assert(string.IsNullOrEmpty(property));

            return configurationName;
        }
    }
}