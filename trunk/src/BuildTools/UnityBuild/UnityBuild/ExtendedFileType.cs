using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class ExtendedFileType : FileType
    {
        private readonly PrecompiledHeaderOptions _precompiledHeaderOptions;

     
        public ExtendedFileType(PrecompiledHeaderOptions precompiledHeaderOptions)
            : base()
        {
            this._precompiledHeaderOptions = precompiledHeaderOptions;
        }

        public PrecompiledHeaderOptions PrecompiledHeaderOption
        {
            get { return _precompiledHeaderOptions; }
            // set { _precompiledHeaderOptions = value; }
        }
    }
}
