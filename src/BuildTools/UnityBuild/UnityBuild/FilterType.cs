using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public partial class FilterType : IFilterOrFile
    {
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
