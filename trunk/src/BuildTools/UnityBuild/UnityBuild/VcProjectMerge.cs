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
        private readonly List<string> _buildConfigurationExcluded;

        public VcProjectMerge(VcProject project)
        {
            Debug.Assert(project != null);

            _project = project;
            _buildConfigurationExcluded = new List<string>();
        }

        //public void ExcludeBuildConfiguration(string buildConfiguration)
        //{
        //    _buildConfigurationExcluded.Add(buildConfiguration);
        //}

        //public void ExcludeBuildConfigurations(IEnumerable<string> buildConfigurations)
        //{
        //    _buildConfigurationExcluded.AddRange(buildConfigurations);
        //}

        public void ExcludeFromBuild(AbstractProjectConfigurationNameConverter projectConverter)
        {
            foreach (ConfigurationType configuration in _project.Details.Configurations)
            {
                string configurationName = AbstractConfigurationNameConverter.GetConfiguration(configuration.Name);
                if (projectConverter.IsNewName(configurationName) == true)
                    continue;

                _buildConfigurationExcluded.Add(configuration.Name);
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
                FilterMerge filterMerge = new FilterMerge(_project.Summary, filter);
                filterMerge.ExcludeBuildConfigurations(_buildConfigurationExcluded);
                itemsAdded.AddRange(filterMerge.Merge());
            }

            // TODO: 하드코딩  

            // FilesMerge filesMerge = new FilesMerge(_project.Summary, newFilter, Files.ToList());
            FilesMerge filesMerge = new FilesMerge(_project.Summary, Files.ToList());
            filesMerge.ExcludeBuildConfigurations(_buildConfigurationExcluded);

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
