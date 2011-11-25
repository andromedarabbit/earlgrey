using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using IniParser;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{

    /// <summary>
    /// Get section names with the number of their key/value pairs from the target .ini file. The global section which has no section name will be shown as '[]'.
    /// </summary>
    /// <example>
    /// <code lang="ini" title="An ini file only with a global section." source=".\Samples\Data\GolobalSectionOnly.ini" />
    /// <code lang="xml" title="Getting a global section from an ini file." source=".\Samples\msbuild-inifile-getsections-only-with-globalsection.xml" />
    /// </example>
    /// <remarks></remarks>
    public class GetSections : AbstractIniFile
    {
        private readonly List<TaskItem> _sections;

        /// <summary>
        /// Initializes a new instance of the <see cref="GetSections"/> class.
        /// </summary>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public GetSections()
        {
            _sections = new List<TaskItem>();
        }

        /// <summary>
        /// Executes the command.
        /// </summary>
        /// <returns></returns>
        /// <inheritdoc/>
        /// <remarks></remarks>
        protected override bool ExecuteCommand()
        {
            if(base.ExecuteCommand() == false)
                return false;

            foreach (SectionData section in Data.Sections)
            {
                string sectionName = section.SectionName;

                if (sectionName == StreamIniDataParser.GlobalSectionName)
                {
                    if(section.Keys.Count == 0)
                        continue;
                    
                    sectionName = GlobalSectionName;
                }

                TaskItem item = new TaskItem(sectionName);
                item.SetMetadata("NumberOfKeys", section.Keys.Count.ToString());

                _sections.Add(item);
            }


            return true;
        }

        /// <summary>
        /// Validates the parameters.
        /// </summary>
        /// <returns></returns>
        /// <inheritdoc/>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            return base.ValidateParameters();
        }

        /// <summary>
        /// [Output] Gets the sections.
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public TaskItem[] Sections 
        { 
            get { return _sections.ToArray(); } 
        }
    }
}
