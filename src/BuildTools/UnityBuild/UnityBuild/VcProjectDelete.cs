using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class VcProjectDelete
    {
        private readonly VcProject _vcProject;
        private readonly string _configurationPlatform;
        private readonly AbstractProjectConfigurationNameConverter _projectConverter;

        public VcProjectDelete(VcProject vcProject, string configurationPlatform, AbstractProjectConfigurationNameConverter projectConverter)
        {
            Debug.Assert(vcProject != null);
            Debug.Assert(string.IsNullOrEmpty(configurationPlatform) == false);
            Debug.Assert(projectConverter != null);

            _vcProject = vcProject;
            _configurationPlatform = configurationPlatform;
            _projectConverter = projectConverter;
        }

        public void DeleteConfigurationPlatform()
        {
            IConfigurationType configuration = _vcProject.GetConfiguration(_configurationPlatform);
            if (configuration == null)
                throw new ArgumentException();

			if (_vcProject.RemoveConfiguration(configuration) == false)
                throw new ApplicationException();

            DeleteConfigurationPlatformInFiles(_vcProject.Files);

            DeleteUnityBuildFilters(_vcProject.Files);

            // _vcProject.Save();
        }


        private static void DeleteUnityBuildFilters(List<object> items)
        {
			var result = items.Where(item => item is IFilterType)
				.Select(item => (IFilterType)item)
                .Where(item => item.Name == "UnityBuild").ToList();
			foreach (IFilterType filter in result)
            {
                items.Remove(filter);
            }

			var filters = items.Where(item => item is IFilterType)
                .Select(item => (IFilterType)item)
                ;
			foreach (IFilterType filterType in filters)
            {
                DeleteUnityBuildFilters(filterType.Items);
            }
            
        }

        private void DeleteConfigurationPlatformInFiles(List<object> items)
        {
            // foreach (object item in items)
            for (int i = 0; i < items.Count(); i++)
            {
                object item = items[i];

				Debug.Assert((item is IFileType) || (item is IFilterType));
                if (item is IFileType)
                {
                    IFileType file = (IFileType)item;
                    DeleteConfigurationPlatformoInFileBuildConfiguration(file);
                }

                if (item is IFilterType)
                {
                    IFilterType filter = (IFilterType)item;
                    DeleteConfigurationPlatformInFiles(filter.Items);                    
                }
            }
        }

        private void DeleteConfigurationPlatformoInFileBuildConfiguration(IFileType file)
        {
            List<IBuildConfigurationType> buildConfigurations = 
                file.Items
				.Where(item => item is IBuildConfigurationType)
				.Select(item => item as IBuildConfigurationType)
                .ToList();

			IEnumerable<IBuildConfigurationType> result = 
                buildConfigurations.Where(
                    item => _projectConverter.IsNewName(
                        AbstractConfigurationNameConverter.GetConfiguration(item.Name)
                                ) == true
                );

			foreach (IBuildConfigurationType buildConfiguration in result)
            {
                file.Items.Remove(buildConfiguration);    
            }
            

            // string projectConfiguration =
            // AbstractConfigurationNameConverter.GetConfiguration(projectConfigurationPlatform);

        }
    }
}