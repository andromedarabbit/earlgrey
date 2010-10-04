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
        private readonly Project _projectSummary;
        private VisualStudioProjectType _projectDetails;

        public VcProject(Project projectSummary)
        {
            Debug.Assert(projectSummary != null);
            Debug.Assert(
                projectSummary.ProjectTypeGuid == KnownProjectTypeGuid.VisualC
                );
            _projectSummary = projectSummary;
        }

        public void Initialize()
        {
            Debug.Assert(File.Exists(_projectSummary.FullPath));

            using (TextReader tr = new StreamReader(_projectSummary.FullPath))
            {
                _projectDetails = (VisualStudioProjectType)(
                    new XmlSerializer(typeof(VisualStudioProjectType))
                ).Deserialize(tr);

                Debug.Assert(_projectDetails != null);
            }
        }

        public VisualStudioProjectType Details
        {
            get
            {
                Debug.Assert(_projectDetails != null);
                return _projectDetails;
            }
        }

        public Project Summary
        {
            get 
            { 
                Debug.Assert(_projectSummary != null);
                return _projectSummary;
            }
        }


    }
}
