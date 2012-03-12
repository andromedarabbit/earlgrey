using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
	internal class FilterTypeFactory
	{
		internal static IFilterType CreateInstance(VisualStudioVersions version, string name)
		{
            if (version == VisualStudioVersions.V2008)
            {
                var filterType = new UnityBuild.VS2008.FilterType(name);
                filterType.ItemsSpecified = true;
                return filterType;
            }

			throw new ArgumentException(
				string.Format("Unknown version of Visual Studio: {0}", version)
				);
		}
	}
}
