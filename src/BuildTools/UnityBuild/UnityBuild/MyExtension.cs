using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
	internal static class MyExtensions
	{
		internal static bool IsVcProject(this Project project)
		{
			return project.FullPath.EndsWith(".vcproj", StringComparison.CurrentCultureIgnoreCase)
				   || project.FullPath.EndsWith(".vcxproj", StringComparison.CurrentCultureIgnoreCase)
				   ;
		}

		internal static bool IsVcProjectRelated(this Project project)
		{
			return IsVcProject(project)
				   || project.FullPath.EndsWith(".vcxproj.filters", StringComparison.CurrentCultureIgnoreCase)
				   ;
		}
	}   
}
