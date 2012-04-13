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
        private readonly bool _groupByFilter;       
        private readonly VcProject _project;
        private readonly AbstractProjectConfigurationNameConverter _projectConverter;
        private readonly List<string> _buildConfigurations;
        private readonly List<string> _buildConfigurationsExcluded;

        private readonly IFilterType _unityBuildFilter;

        private readonly int _maxFilesPerFile;

        public VcProjectMerge(
            VcProject project
            , AbstractProjectConfigurationNameConverter projectConverter
            )
            : this(project, projectConverter, true, 0)
        {
        }

        public VcProjectMerge(            
            VcProject project
            , AbstractProjectConfigurationNameConverter projectConverter
            , bool groupByFilter
            , int maxFilesPerFile
            )
        {
            Debug.Assert(project != null);

            _project = project;
            _buildConfigurations = new List<string>();
            _buildConfigurationsExcluded = new List<string>();

            _buildConfigurations.AddRange(_project.ConfigurationPlatformNames);

            _projectConverter = projectConverter;
            _groupByFilter = groupByFilter;

			_unityBuildFilter = FilterTypeFactory.CreateInstance(_project.Version, "UnityBuild");
            // _unityBuildFilter.Name = "UnityBuild";
            // _unityBuildFilter.NameSpecified = true;
            // _unityBuildFilter.ItemsSpecified = true;

            ExcludeFromBuild();

            _maxFilesPerFile = maxFilesPerFile;
        }

        private void ExcludeFromBuild()
        {
            foreach (IConfigurationType configuration in _project.Configurations)
            {
                string configurationName = AbstractConfigurationNameConverter.GetConfiguration(configuration.Name);
                if (_projectConverter.IsNewName(configurationName) == true)
                    continue;

                _buildConfigurationsExcluded.Add(configuration.Name);
            }
        }

        private IEnumerable<IFilterType> Filters
        {
            get
            {
				IEnumerable<IFilterType> filters = from item in _project.Files
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
                IEnumerable<IFileType> files = from item in _project.Files
                                               where item is IFileType
                                               select (IFileType)item
                    ;
                return files;
            }
        }

        public List<IFilterOrFile> Merge()
        {
            if (_groupByFilter == true)
            {
                return MergeInGroupByFilterMode();
            }

            return MergeNotInGroupByFilterMode();
        }

        // TODO: MergeNotInGroupByFilterMode 와 MergeInGroupByFilterMode 가 비슷하다.
        private List<IFilterOrFile> MergeNotInGroupByFilterMode()
        {
            List<IFilterOrFile> itemsAdded = new List<IFilterOrFile>();

            // TODO: 하드코딩  
            List<IFileType> files = GetAllSrcFiles();
            FilesMerge filesMerge = new FilesMerge(_project.Version, _project.Directory, files, _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);

            List<IFileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count == 0)
                return itemsAdded;

            if (_project.Files.Contains(_unityBuildFilter) == false)
            {
                _project.Files.Add(_unityBuildFilter);
            }
            _unityBuildFilter.Items.AddRange(filesAdded.ToArray());
            itemsAdded.AddRange(filesAdded.ToArray());

            return itemsAdded;
        }

        private List<IFileType> GetAllSrcFiles()
        {
            List<IFileType> files = new List<IFileType>();
            GetAllSrcFiles(_project.Files, files);
            return files;
        }

        private static void GetAllSrcFiles(IEnumerable<object> items, List<IFileType> files)
        {
            Debug.Assert(items != null);
            Debug.Assert(files != null);

            foreach (object item in items)
            {
                if (item is IFileType)
                {
                    IFileType file = (IFileType)item;
                    if (file.IsSrcFile)
                        files.Add(file);
                }

                if (item is IFilterType)
                {
                    IFilterType filter = item as IFilterType;
                    GetAllSrcFiles(filter.Items, files);
                }
            }
        }

        private List<IFilterOrFile> MergeInGroupByFilterMode()
        {
            List<IFilterOrFile> itemsAdded = new List<IFilterOrFile>();

            foreach (var filter in Filters)
            {
                FilterMerge filterMerge = new FilterMerge(_project.Version, _project.Directory, filter, _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);
                itemsAdded.AddRange(filterMerge.Merge());
            }

            // TODO: 하드코딩  
            FilesMerge filesMerge = new FilesMerge(_project.Version, _project.Directory, Files.ToList(), _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);


            List<IFileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count > 0)
            {
                if (_project.Files.Contains(_unityBuildFilter) == false)
                {
                    _project.Files.Add(_unityBuildFilter);
                }
                itemsAdded.AddRange(filesAdded.ToArray());
            }

            return itemsAdded;
        }
    }
}