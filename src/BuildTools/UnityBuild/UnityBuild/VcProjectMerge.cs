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

        private readonly FilterType _unityBuildFilter;
        
        private readonly int _maxFilesPerFile;

        public VcProjectMerge(VcProject project, AbstractProjectConfigurationNameConverter projectConverter)
            : this(project, projectConverter, true, 0)
        {
            
        }

        public VcProjectMerge(VcProject project, AbstractProjectConfigurationNameConverter projectConverter, bool groupByFilter, int maxFilesPerFile)
        {
            Debug.Assert(project != null);

            _project = project;
            _buildConfigurations = new List<string>();
            _buildConfigurationsExcluded = new List<string>();

            _buildConfigurations.AddRange(_project.ConfigurationPlatformNames);

            _projectConverter = projectConverter;
            _groupByFilter = groupByFilter;

            _unityBuildFilter = new FilterType();
            _unityBuildFilter.Name = "UnityBuild";
            _unityBuildFilter.NameSpecified = true;
            _unityBuildFilter.ItemsSpecified = true;

            ExcludeFromBuild();

            _maxFilesPerFile = maxFilesPerFile;
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
            List<FileType> files = GetAllSrcFiles();
            FilesMerge filesMerge = new FilesMerge(_project.Directory, files, _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);

            List<FileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count == 0)
                return itemsAdded;
            
            if (_project.Details.Files.Contains(_unityBuildFilter) == false)
            {
                _project.Details.Files.Add(_unityBuildFilter);
            }
            _unityBuildFilter.Items.AddRange(filesAdded.ToArray());
            itemsAdded.AddRange(filesAdded.ToArray());
        
            return itemsAdded;
        }

        private List<FileType> GetAllSrcFiles()
        {
            List<FileType> files = new List<FileType>();
            GetAllSrcFiles(_project.Details.Files, files);
            return files;
        }

        private void GetAllSrcFiles(IEnumerable<object> items, List<FileType> files)
        {
            Debug.Assert(items != null);
            Debug.Assert(files != null);

            foreach (object item in items)
            {
                if(item is FileType)
                {
                    FileType file = (FileType) item;
                    if(file.IsSrcFile)
                        files.Add(file);
                }

                if(item is FilterType)
                {
                    FilterType filter = item as FilterType;
                    GetAllSrcFiles(filter.Items, files);
                }
            }
        }

        private List<IFilterOrFile> MergeInGroupByFilterMode()
        {
            List<IFilterOrFile> itemsAdded = new List<IFilterOrFile>();

            foreach (var filter in Filters)
            {
                FilterMerge filterMerge = new FilterMerge(_project.Directory, filter, _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);
                itemsAdded.AddRange(filterMerge.Merge());
            }

            // TODO: 하드코딩  
            FilesMerge filesMerge = new FilesMerge(_project.Directory, Files.ToList(), _buildConfigurations, _buildConfigurationsExcluded, _maxFilesPerFile);
            

            List<FileType> filesAdded = filesMerge.Merge();
            if (filesAdded.Count > 0)
            {
                if (_project.Details.Files.Contains(_unityBuildFilter) == false)
                {
                    _project.Details.Files.Add(_unityBuildFilter);
                }
                itemsAdded.AddRange(filesAdded.ToArray());
            }

            return itemsAdded;
        }

    }
}
