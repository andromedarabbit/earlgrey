using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Xml.Serialization;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class ProjectConverter
    {
        private readonly Project _originalProject;
        private Project _convertedProject;

        public ProjectConverter(Project project)
        {
            Debug.Assert(_originalProject != null);
            _originalProject = project;
        }

        public void Run()
        {
            // SolutionFile slnFile = SolutionFile.FromFile(solutionFileFullPath);
            using (TextReader tr = new StreamReader(_originalProject.FullPath))
            {
                // Now deserialize.
                var project = (VisualStudioProjectType)(
                    new XmlSerializer(typeof(VisualStudioProjectType))
                ).Deserialize(tr);

                Debug.Assert(project != null);
            }

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
