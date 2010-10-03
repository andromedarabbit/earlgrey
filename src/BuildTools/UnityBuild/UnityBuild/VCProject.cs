using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class VcProject
    {
        private readonly Project _project;

        public VcProject(Project project)
        {
            Debug.Assert(project != null);
            Debug.Assert(
                project.ProjectTypeGuid == KnownProjectTypeGuid.VisualC
                );
            _project = project;
        }

        public void Run()
        {
            // SolutionFile slnFile = SolutionFile.FromFile(solutionFileFullPath);
            using (TextReader tr = new StreamReader(_project.FullPath))
            {
                // Now deserialize.
                var project = (VisualStudioProjectType)(
                    new XmlSerializer(typeof(VisualStudioProjectType))
                ).Deserialize(tr);

                Debug.Assert(project != null);
            }
        }


    }
}
