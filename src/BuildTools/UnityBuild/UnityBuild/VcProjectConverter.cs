using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class VcProjectConverter
    {
        private readonly SolutionFile _originalSolution;
        private readonly Project _originalProject;
        private Project _convertedProject;

        public VcProjectConverter(SolutionFile solution, Project project)
        {
            Debug.Assert(solution != null);
            Debug.Assert(project != null);

            _originalSolution = solution;
            _originalProject = project;
        }

        public void Run()
        {
            // NOTE 사본을 만드는 메서드 Clone 같은 게 없으니 직접 똑같은 인스턴스를 생성하는 수밖에
            _convertedProject = new Project(_originalSolution, _originalProject);
        }

        public Project OriginalProject
        {
            get { return _originalProject; }
        }

        public Project ConvertedProject
        {
            get
            {
                if (_convertedProject == null)
                    throw new ApplicationException();
                return _convertedProject;
            }
        }
    }
}