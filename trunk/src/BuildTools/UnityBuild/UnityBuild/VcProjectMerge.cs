using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class VcProjectMerge
    {
        private readonly VcProject _project;

        public VcProjectMerge(VcProject project)
        {
            Debug.Assert(project != null);

            _project = project;
        }

        private IEnumerable<FilterType> Filters
        {
            get
            {
                IEnumerable<FilterType> filters = from item in _project.Details.Files
                                                  where item is FilterType
                                                  select (FilterType)item
                                              ;
                return filters;
            }
        }

        private IEnumerable<FileType> Files
        {
            get
            {
                IEnumerable<FileType> files = from item in _project.Details.Files
                                              where item is FileType
                                              select (FileType)item
                                              ;
                return files;
            }
        }

        public void Merge()
        {
            foreach (var filter in Filters)
            {
                FilterMerge filterMerge = new FilterMerge(_project.Summary, filter);
                filterMerge.Merge();
            }

            // TODO: 하드코딩
            FilterType newFilter = new FilterType();
            newFilter.Name = "UnityBuild";
            _project.Details.Files.Add(newFilter);

            FilesMerge filesMerge = new FilesMerge(_project.Summary, newFilter, Files.ToList());
            filesMerge.Merge();
        }
    }
}
