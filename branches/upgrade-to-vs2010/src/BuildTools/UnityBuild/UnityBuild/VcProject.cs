using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using CWDev.SLNTools.Core;

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

        public VisualStudioProjectType Details
        {
            get
            {
                Debug.Assert(_projectDetails != null);
                return _projectDetails;
            }
        }

        public Project Summary
        {
            get
            {
                Debug.Assert(_projectSummary != null);
                return _projectSummary;
            }
        }

        public string Directory
        {
            get { return Path.GetDirectoryName(this._projectSummary.FullPath); }
        }

        public PropertyLineHashList ConfigurationPlatforms
        {
            get { return this.Summary.ProjectConfigurationPlatformsLines; }
        }

        public IEnumerable<string> ConfigurationPlatformNames
        {
            get { return this.Details.ConfigurationPlatformNames; }
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

        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform, FileType file)
        {
            return _projectDetails.GetPrecompiledHeaderOption(configurationPlatform, file);
        }

        // 이름은 Headers 인데 실제론 /Yc 옵션이 적용된 .cpp 파일을 반환한다.
        public IEnumerable<FileType> GetPrecompiledHeaders(string configurationPlatform)
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
			return FindFile(this.Details.Files, fileName);
		}

		private static FileType FindFile(IEnumerable<object> items, string fileName)
		{
			foreach (object item in items)
			{
				if (item is FileType)
				{
					FileType file = (FileType)item;
					if (file.FileName.Equals(fileName, StringComparison.CurrentCultureIgnoreCase) == true)
						return file;
				}

				if (item is FilterType)
				{
					FilterType filter = (FilterType)item;
					FileType fileFound = FindFile(filter.Items, fileName);
					if (fileFound != null)
						return fileFound;
				}
			}

			return null;
		}

		public FilterType FindFilter(string filterName)
		{
			return FindFilter(this.Details.Files, filterName);
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
    }
}