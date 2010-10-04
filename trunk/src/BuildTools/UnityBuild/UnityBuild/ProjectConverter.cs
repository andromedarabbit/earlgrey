using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class ProjectConverter
    {
        private readonly Project _originalProject;
        private Project _convertedProject;

        public ProjectConverter(Project project)
        {
            Debug.Assert(project != null);
            _originalProject = project;
        }

        public void Run()
        {
            _convertedProject = null;
        }

        public Project OriginalProject
        {
            get { return _originalProject; }
        }

        public Project ConvertedProject
        {
            get
            {
                if(_convertedProject == null)
                    throw new ApplicationException();
                return _convertedProject;
            }
        }
        


    }
}
