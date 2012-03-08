using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public interface IFileType : IFilterOrFile
    {
        bool ExcludedFromBuild(string buildConfigurationName);

        bool IsSrcFile
        {
            get;
        }

        PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform);

        void SetPrecompiledHeaderOption(string configurationPlatform, PrecompiledHeaderOptions options);

        void ExcludeFromBuild(string buildConfigurationName);

        void IncludeInBuild(string buildConfigurationName);

        string FileName
        {
            get;
        }

        string RelativePath
        {
            get;
        }

        string RelativeDir
        {
            get;
        }

        void AddPrecompiledHeaderOptions(string configurationPlatform, PrecompiledHeaderOptions options);

        PrecompiledHeaderOptions GetPrecompiledHeaderOptions(string configurationPlatform);

        IDictionary<string, PrecompiledHeaderOptions> GetPrecompiledHeaderOptions();

    }
}
