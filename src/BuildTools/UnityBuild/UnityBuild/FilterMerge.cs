﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class FilterMerge
    {
        private readonly VisualStudioVersions _version;
        private readonly string _projectDirectory;
        private readonly IFilterType _filter;

        private readonly List<string> _buildConfigurationsExcluded;
        private readonly List<string> _buildConfigurations;

        private readonly int _maxFilesPerFile;

        public FilterMerge(VcProject project, IFilterType filter)
            : this(project.Version, project.Directory, filter, project.ConfigurationPlatformNames)
        {
            
        }

        public FilterMerge(
            VisualStudioVersions version
            , string projectDirectory
            , IFilterType filter
            , IEnumerable<string> buildConfigurations
            )
            : this(version, projectDirectory, filter, buildConfigurations, new List<string>(), 0)
        {
        }

        public FilterMerge(
            VcProject project            
            , IFilterType filter
            , IEnumerable<string> buildConfigurationsExcluded
            , int maxFilesPerFile
            )
            : this(project.Version, project.Directory, filter, project.ConfigurationPlatformNames, new List<string>(), 0)
        {

        }

        public FilterMerge(
            VisualStudioVersions version
            , string projectDirectory
            , IFilterType filter
            , IEnumerable<string> buildConfigurations
            , IEnumerable<string> buildConfigurationsExcluded
            , int maxFilesPerFile
            )
        {
            Debug.Assert(version != VisualStudioVersions.None);
            Debug.Assert(string.IsNullOrEmpty(projectDirectory) == false);
            Debug.Assert(filter != null);

            _version = version;
            _projectDirectory = projectDirectory;
            _filter = filter;

            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);

            _maxFilesPerFile = maxFilesPerFile;
        }

        private IEnumerable<IFilterType> Filters
        {
            get
            {
                IEnumerable<IFilterType> filters = from item in _filter.Items
                                                   where item is IFilterType
                                                   select (IFilterType)item
                    ;
                return filters;
            }
        }

        private IEnumerable<IFileType> Files
        {
            get
            {
                IEnumerable<IFileType> files = from item in _filter.Items
                                               where item is IFileType
                                               select (IFileType)item
                    ;
                return files;
            }
        }

        public List<IFilterOrFile> Merge()
        {
            List<IFilterOrFile> filesOrFiltersAdded = new List<IFilterOrFile>();

            foreach (var filter in Filters)
            {
                FilterMerge filterMerge = new FilterMerge(_version, _projectDirectory, filter, _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);
                filesOrFiltersAdded.AddRange(filterMerge.Merge());
            }

            // TODO: 하드코딩
            FilesMerge filesMerge = new FilesMerge(_version, _projectDirectory, Files.ToList(), _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);

            List<IFileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count > 0)
            {
				IFilterType parentFilter = GetParentFilter(filesOrFiltersAdded);
                parentFilter.Items.AddRange(filesAdded.ToArray());
                filesOrFiltersAdded.AddRange(filesAdded.ToArray());
            }
            return filesOrFiltersAdded;
        }

        private IFilterType GetParentFilter(List<IFilterOrFile> filesOrFiltersAdded)
        {
			IFilterType newFilter = null;

            var result = _filter.Items
				.Where(item => item is IFilterType && ((IFilterType)item).Name.Equals("UnityBuild") == true)
				.Select(item => (IFilterType)item);
            if (result.Count() > 0)
            {
                Debug.Assert(result.Count() == 1);
                newFilter = result.First();
            }

            if (newFilter == null)
            {
                newFilter = FilterTypeFactory.CreateInstance(_version, "UnityBuild");
                // newFilter = new FilterType();
                // newFilter.Name = "UnityBuild";
                // newFilter.NameSpecified = true;
                // newFilter.ItemsSpecified = true;

                _filter.Items.Add(newFilter);
                filesOrFiltersAdded.Add(newFilter);
            }

            return newFilter;
        }
    }
}