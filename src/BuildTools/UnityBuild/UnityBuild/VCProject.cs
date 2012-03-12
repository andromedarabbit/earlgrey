using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using CWDev.SLNTools.Core;

using UnityBuild.VS2008; // 임시

namespace UnityBuild
{
    public class VcProject
    {
        private readonly Project _projectSummary;
        private VisualStudioProjectType _projectDetails;

        public VcProject(Project projectSummary)
        {
            Debug.Assert(projectSummary != null);
            Debug.Assert(
                projectSummary.ProjectTypeGuid == KnownProjectTypeGuid.VisualC
                );

            _projectSummary = projectSummary;
        }

        public void Load()
        {
            Debug.Assert(File.Exists(_projectSummary.FullPath));
            Debug.Assert(_projectDetails == null);

            _projectDetails = VisualStudioProjectType.LoadFromFile(_projectSummary.FullPath, Encoding.Default);
            Debug.Assert(_projectDetails != null);

            _projectDetails.Initialize();
        }

        public void Save()
        {
            _projectDetails.SaveToFile(_projectSummary.FullPath, Encoding.UTF8);
        }

        public IEnumerable<IPlatform> Platforms
        {
            get
            {
                Debug.Assert(_projectDetails != null);
                return _projectDetails.Platforms.Cast<IPlatform>();
            }
        }

		public IEnumerable<IConfigurationType> Configurations
        {
            get
            {
                Debug.Assert(_projectDetails != null);
				return _projectDetails.Configurations.Cast<IConfigurationType>();
            }
        }

		public void AddConfiguration(IConfigurationType configurationType)
		{
			Debug.Assert(_projectDetails != null);
			_projectDetails.Configurations.Add(configurationType as ConfigurationType);
		}

		public bool RemoveConfiguration(IConfigurationType configurationType)
		{
			Debug.Assert(_projectDetails != null);
			return _projectDetails.Configurations.Remove(configurationType as ConfigurationType);
		}

        public List<object> Files
        {
            get
            {
                Debug.Assert(_projectDetails != null);
                return _projectDetails.Files;
            }
        }

        public string Name
        {
            get
            {
                Debug.Assert(_projectSummary != null);
                return _projectSummary.ProjectName;
            }
        }

        public string Guid
        {
            get
            {
                Debug.Assert(_projectSummary != null);
                return _projectSummary.ProjectGuid;
            }
        }

        public string FullPath
        {
            get
            {
                Debug.Assert(_projectSummary != null);
                return _projectSummary.FullPath;
            }
        }

        public string Directory
        {
            get { return Path.GetDirectoryName(this._projectSummary.FullPath); }
        }

        public PropertyLineHashList ConfigurationPlatforms
        {
            get { return _projectSummary.ProjectConfigurationPlatformsLines; }
        }

        public IEnumerable<string> ConfigurationPlatformNames
        {
            get { return _projectDetails.ConfigurationPlatformNames; }
        }

        public bool HasConfigurationPlatform(string configurationPlatformName)
        {
            return _projectDetails.HasConfigurationPlatform(configurationPlatformName);
        }

        internal
            void SetPrecompiledHeaderOption(string configurationPlatform, PrecompiledHeaderOptions options,
                                            FileType file)
        {
            _projectDetails.SetPrecompiledHeaderOption(configurationPlatform, options, file);
        }

        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform)
        {
            return _projectDetails.GetPrecompiledHeaderOption(configurationPlatform);
        }

        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform, IFileType file)
        {
            return _projectDetails.GetPrecompiledHeaderOption(configurationPlatform, file);
        }

        // 이름은 Headers 인데 실제론 /Yc 옵션이 적용된 .cpp 파일을 반환한다.
        public IEnumerable<IFileType> GetPrecompiledHeaders(string configurationPlatform)
        {
            return _projectDetails.GetPrecompiledHeaders(configurationPlatform);
        }

        public void DeleteConfigurationPlatform(string name)
        {
            _projectDetails.DeleteConfigurationPlatform(name);
        }

        internal bool HasConfiguration(string name)
        {
            return GetConfiguration(name) != null;
        }

        internal ConfigurationType GetConfiguration(string srcName)
        {
            return _projectDetails.GetConfiguration(srcName);
        }

        // TODO: projectConverter 대신 깔끔하게 delegate 를 매개변수로 받는 게 낫지 않을까?
        public void ExcludeFromBuild(AbstractProjectConfigurationNameConverter projectConverter)
        {
            _projectDetails.ExcludeFromBuild(projectConverter);
        }

        public FileType FindFile(string fileName)
        {
            return FindFile(this.Files, fileName) as FileType;
        }

        private static IFileType FindFile(IEnumerable<object> items, string fileName)
        {
            foreach (object item in items)
            {
                if (item is IFileType)
                {
                    IFileType file = (IFileType)item;
                    if (file.Name.Equals(fileName, StringComparison.CurrentCultureIgnoreCase) == true)
                        return file;
                }

                if (item is IFilterType)
                {
                    IFilterType filter = (IFilterType)item;
                    IFileType fileFound = FindFile(filter.Items, fileName);
                    if (fileFound != null)
                        return fileFound;
                }
            }

            return null;
        }

        public IFilterType FindFilter(string fileName)
        {
            return FindFilter(this.Files, fileName) as IFilterType;
        }

        private static FilterType FindFilter(IEnumerable<object> items, string filterName)
        {
            foreach (object item in items)
            {
                if (item is FilterType)
                {
                    FilterType filter = (FilterType)item;
                    if (filter.Name.Equals(filterName, StringComparison.CurrentCultureIgnoreCase) == true)
                        return filter;

                    FilterType filterFound = FindFilter(filter.Items, filterName);
                    if (filterFound != null)
                        return filterFound;
                }
            }

            return null;
        }

		public VisualStudioVersions Version
		{
			get
			{
				// TODO: 나중에 정교하게 고쳐야 함
				if (this._projectSummary.FullPath.EndsWith(".vcproj") == true)
				{
					return VisualStudioVersions.V2008;
				}

				if (this._projectSummary.FullPath.EndsWith(".vcxproj") == true)
				{
					return VisualStudioVersions.V2010;
				}

				throw new ApplicationException(
                    string.Format("Could not identify the version of this solution file: {0}", _projectSummary.FullPath)
					);
			}
		}
    }
}