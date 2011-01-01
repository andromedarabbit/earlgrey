using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class VcProjectMerge
    {
        private readonly VcProject _project;        
        private readonly AbstractProjectConfigurationNameConverter _projectConverter;
        private readonly List<string> _buildConfigurations;
        private readonly List<string> _buildConfigurationsExcluded;

        public VcProjectMerge(VcProject project, AbstractProjectConfigurationNameConverter projectConverter)
        {
            Debug.Assert(project != null);

            _project = project;
            _buildConfigurations = new List<string>();
            _buildConfigurationsExcluded = new List<string>();

            _buildConfigurations.AddRange(_project.ConfigurationPlatformNames);

            _projectConverter = projectConverter;
            ExcludeFromBuild();
        }

        private void ExcludeFromBuild()
        {
            foreach (ConfigurationType configuration in _project.Details.Configurations)
            {
                string configurationName = AbstractConfigurationNameConverter.GetConfiguration(configuration.Name);
                if (_projectConverter.IsNewName(configurationName) == true)
                    continue;

                _buildConfigurationsExcluded.Add(configuration.Name);
            }
        }

        private IEnumerable<FilterType> Filters
        {
            get
            {
                IEnumerable<FilterType> filters = from item in _project.Details.Files
                                                  where item is FilterType
                                                  select (FilterType)item
                                              ;
                return filters;
            }
        }

        private IEnumerable<FileType> Files
        {
            get
            {
                IEnumerable<FileType> files = from item in _project.Details.Files
                                                      where item is FileType
                                                      select (FileType)item
                                              ;
                return files;
            }
        }

        public List<IFilterOrFile> Merge()
        {
            List<IFilterOrFile> itemsAdded = new List<IFilterOrFile>();

            foreach (var filter in Filters)
            {
                FilterMerge filterMerge = new FilterMerge(_project.Directory, filter, _buildConfigurations, _buildConfigurationsExcluded);
                itemsAdded.AddRange(filterMerge.Merge());
            }

            // TODO: 하드코딩  
            FilesMerge filesMerge = new FilesMerge(_project.Directory, Files.ToList(), _buildConfigurations, _buildConfigurationsExcluded);
      
            List<FileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count > 0)
            {
                FilterType newFilter = new FilterType();
                newFilter.Name = "UnityBuild";
                newFilter.NameSpecified = true;
                newFilter.ItemsSpecified = true;

                _project.Details.Files.Add(newFilter);
                itemsAdded.AddRange(filesAdded.ToArray());
            }

            return itemsAdded;
        }
    }
}
