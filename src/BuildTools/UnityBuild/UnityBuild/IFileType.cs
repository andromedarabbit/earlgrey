using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public interface IFileType : IFilterOrFile
    {
        // static IFileType CreateInstance();


        bool ExcludedFromBuild(string buildConfigurationName);

        bool IsSrcFile
        {
            get;
        }

        PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform);

        void SetPrecompiledHeaderOption(string configurationPlatform, PrecompiledHeaderOptions options);

        void ExcludeFromBuild(string buildConfigurationName);

        void IncludeInBuild(string buildConfigurationName);

        //string Name
        //{
        //    get;
        //}

        string RelativePath
        {
            get;
        }

        string RelativeDir
        {
            get;
        }

        IEnumerable<string> BuildConfigurationsWhenExcludedFromBuild
        {
            get;
        }

        void AddPrecompiledHeaderOptions(string configurationPlatform, PrecompiledHeaderOptions options);

        PrecompiledHeaderOptions GetPrecompiledHeaderOptions(string configurationPlatform);

        IDictionary<string, PrecompiledHeaderOptions> GetPrecompiledHeaderOptions();

    }
}
