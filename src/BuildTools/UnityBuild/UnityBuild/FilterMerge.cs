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
        private readonly string _projectDirectory;
        private readonly FilterType _filter;

        private readonly List<string> _buildConfigurationsExcluded;
        private readonly List<string> _buildConfigurations;


        public FilterMerge(string projectDirectory, FilterType filter, IEnumerable<string> buildConfigurations)
            : this(projectDirectory, filter, buildConfigurations, new List<string>())
        {
            
        }

        public FilterMerge(string projectDirectory, FilterType filter, IEnumerable<string> buildConfigurations, IEnumerable<string> buildConfigurationsExcluded)
        {
            Debug.Assert(string.IsNullOrEmpty(projectDirectory) == false);
            Debug.Assert(filter != null);

            _projectDirectory = projectDirectory;
            _filter = filter;
            
            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);
        }

        //public void ExcludeBuildConfiguration(string buildConfiguration)
        //{
        //    _buildConfigurationsExcluded.Add(buildConfiguration);
        //}

        //public void ExcludeBuildConfigurations(IEnumerable<string> buildConfigurations)
        //{
        //    _buildConfigurationsExcluded.AddRange(buildConfigurations);
        //}

        //public void BuildConfiguration(string buildConfiguration)
        //{
        //    _buildConfigurations.Add(buildConfiguration);
        //}

        //public void BuildConfiguration(IEnumerable<string> buildConfigurations)
        //{
        //    _buildConfigurations.AddRange(buildConfigurations);
        //}

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
                FilterMerge filterMerge = new FilterMerge(_projectDirectory, filter, _buildConfigurations, _buildConfigurationsExcluded);
                // filterMerge.ExcludeBuildConfigurations(_buildConfigurationsExcluded);
                filesOrFiltersAdded.AddRange(filterMerge.Merge());
            }
            
            // TODO: 하드코딩
            // FilesMerge filesMerge = new FilesMerge(_project, newFilter, Files.ToList());
            FilesMerge filesMerge = new FilesMerge(_projectDirectory, Files.ToList(), _buildConfigurations, _buildConfigurationsExcluded);
            // filesMerge.ExcludeBuildConfigurations(_buildConfigurationsExcluded);

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
