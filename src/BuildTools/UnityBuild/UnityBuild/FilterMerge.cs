using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class FilterMerge
    {
        private readonly string _projectDirectory;
        private readonly FilterType _filter;

        private readonly List<string> _buildConfigurationsExcluded;
        private readonly List<string> _buildConfigurations;

        // private readonly FilterType _rootFilter;

        public FilterMerge(string projectDirectory, FilterType filter, IEnumerable<string> buildConfigurations)
            : this(projectDirectory, filter, buildConfigurations, new List<string>())
            // : this(projectDirectory, filter, buildConfigurations, new List<string>(), null)
        {
            
        }

        public FilterMerge(string projectDirectory, FilterType filter, IEnumerable<string> buildConfigurations, IEnumerable<string> buildConfigurationsExcluded)
        //    : this(projectDirectory, filter, buildConfigurations, buildConfigurationsExcluded, null)
        //{
            
        //}

        //public FilterMerge(string projectDirectory, FilterType filter, IEnumerable<string> buildConfigurations, IEnumerable<string> buildConfigurationsExcluded, FilterType rootFilter)
        {
            Debug.Assert(string.IsNullOrEmpty(projectDirectory) == false);
            Debug.Assert(filter != null);

            _projectDirectory = projectDirectory;
            _filter = filter;
            
            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);

            // _rootFilter = rootFilter;
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
                FilterMerge filterMerge = new FilterMerge(_projectDirectory, filter, _buildConfigurations, _buildConfigurationsExcluded); //, _rootFilter);
                filesOrFiltersAdded.AddRange(filterMerge.Merge());
            }
            
            // TODO: 하드코딩
            FilesMerge filesMerge = new FilesMerge(_projectDirectory, Files.ToList(), _buildConfigurations, _buildConfigurationsExcluded);
      
            List<FileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count > 0)
            {
                FilterType parentFilter = GetParentFilter(filesOrFiltersAdded);
                parentFilter.Items.AddRange(filesAdded.ToArray());
                filesOrFiltersAdded.AddRange(filesAdded.ToArray());
            }
            return filesOrFiltersAdded;
        }

        private FilterType GetParentFilter(List<IFilterOrFile> filesOrFiltersAdded)
        {
            //if (_rootFilter != null)
            //    return _rootFilter;

            FilterType newFilter = null;

            var result = _filter.Items
                .Where(item => item is FilterType && ((FilterType)item).Name.Equals("UnityBuild") == true)
                .Select(item => (FilterType)item);
            if(result.Count() > 0)
            {
                Debug.Assert(result.Count() == 1);
                newFilter = result.First();
            }

            if (newFilter == null)
            {
                newFilter = new FilterType();
                newFilter.Name = "UnityBuild";
                newFilter.NameSpecified = true;
                newFilter.ItemsSpecified = true;

                _filter.Items.Add(newFilter);
                filesOrFiltersAdded.Add(newFilter);
            }

            return newFilter;
        }
    }
}
