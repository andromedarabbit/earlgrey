using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class FilterMerge
    {
        private readonly Project _project;
        private readonly FilterType _filter;

        private readonly List<string> _buildConfigurationExcluded;


        public FilterMerge(Project project, FilterType filter)
        {
            Debug.Assert(project != null);
            Debug.Assert(filter != null);

            _project = project;
            _filter = filter;
            _buildConfigurationExcluded = new List<string>();
        }

        public void ExcludeBuildConfiguration(string buildConfiguration)
        {
            _buildConfigurationExcluded.Add(buildConfiguration);
        }

        public void ExcludeBuildConfigurations(IEnumerable<string> buildConfigurations)
        {
            _buildConfigurationExcluded.AddRange(buildConfigurations);
        }

        private IEnumerable<FilterType> Filters
        {
            get
            {
                IEnumerable<FilterType> filters = from item in _filter.Items
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
                IEnumerable<FileType> files = from item in _filter.Items
                                              where item is FileType
                                              select (FileType)item
                                              ;
                return files;
            }
        }

        public List<IFilterOrFile> Merge()
        {
            List<IFilterOrFile> filesOrFiltersAdded = new List<IFilterOrFile>();

            foreach (var filter in Filters)
            {
                FilterMerge filterMerge = new FilterMerge(_project, filter);
                filterMerge.ExcludeBuildConfigurations(_buildConfigurationExcluded);
                filesOrFiltersAdded.AddRange(filterMerge.Merge());
            }
            
            // TODO: 하드코딩
            // FilesMerge filesMerge = new FilesMerge(_project, newFilter, Files.ToList());
            FilesMerge filesMerge = new FilesMerge(_project, Files.ToList());
            filesMerge.ExcludeBuildConfigurations(_buildConfigurationExcluded);

            List<FileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count > 0)
            {
                FilterType newFilter = new FilterType();
                newFilter.Name = "UnityBuild";
                newFilter.NameSpecified = true;
                newFilter.ItemsSpecified = true;

                _filter.Items.Add(newFilter);
                newFilter.Items.AddRange(filesAdded.ToArray());
                
                filesOrFiltersAdded.Add(newFilter);
                filesOrFiltersAdded.AddRange(filesAdded.ToArray());
            }
            return filesOrFiltersAdded;
        }
    }
}
