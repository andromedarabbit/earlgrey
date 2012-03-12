using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild.VS2008
{
	public partial class ConfigurationType : IConfigurationType
	{
        //internal IConfigurationType Copy()
        //{
        //    return Clone() as IConfigurationType;
        //}

        public virtual IConfigurationType Copy()
        {
            return Clone() as IConfigurationType;
        }
	}
}
