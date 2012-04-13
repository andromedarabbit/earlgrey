using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild.VS2008
{
    public partial class FilterType : IFilterType
    {
		public FilterType(string name)
			: this()
		{
            this.Name = name;
			this.NameSpecified = !string.IsNullOrEmpty(name);
		}

        #region IFilterOrFile member impletations

        public bool IsFilter
        {
            get { return true; }
        }

        public bool IsFile
        {
            get { return false; }
        }

        #endregion
    }
}