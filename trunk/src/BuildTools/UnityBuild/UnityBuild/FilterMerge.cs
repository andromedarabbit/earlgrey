using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class FilterMerge
    {
        private readonly Project _project;
        private readonly FilterType _filter;

        public FilterMerge(Project project, FilterType filter)
        {
            Debug.Assert(project != null);
            Debug.Assert(filter != null);

            _project = project;
            _filter = filter;
        }

        private IEnumerable<FilterType> Filters
        {
            get
            {
                IEnumerable<FilterType> filters = from item in _filter.Items
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
                IEnumerable<FileType> files = from item in _filter.Items
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
                FilterMerge filterMerge = new FilterMerge(_project, filter);
                filterMerge.Merge();
            }
            
            // TODO: 하드코딩
            FilterType newFilter = new FilterType();
            newFilter.Name = "UnityBuild";
            newFilter.NameSpecified = true;
            _filter.Items.Add(newFilter);

            FilesMerge filesMerge = new FilesMerge(_project, newFilter, Files.ToList());
            filesMerge.Merge();
        }
    }
}
