using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
	public interface IConfigurationType
	{
        string Name { get; set; }
        IConfigurationType Copy();
	}
}
