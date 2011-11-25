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
    /// Get the key/value pairs with the additional comments from the target section of the .ini file.
    /// </summary>
    /// <example>
    /// <code lang="ini" title="An ini file." source=".\Samples\Data\Sections.ini" />
    /// <code lang="xml" title="Getting key/value pairs from the target section." source=".\Samples\msbuild-inifile-getsection.xml" />
    /// </example>
    /// <remarks>Not yet able to handle the section with its name contains the spaces or something.</remarks>
    public class GetSection : AbstractIniFile
    {
        private readonly List<TaskItem> _keyValues;

        /// <summary>
        /// Initializes a new instance of the <see cref="GetSection"/> class.
        /// </summary>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public GetSection()
        {
            _keyValues = new List<TaskItem>();
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

            if (Data.Sections.ContainsSection(InternalSection) == false)
                throw new Exception(
                    string.Format("Section named '{0}' not found!", InternalSection)
                    );

            KeyDataCollection section = Data[InternalSection];
            foreach (KeyData keyData in section)
            {
                TaskItem keyValue = GetKeyValue(keyData);
                _keyValues.Add(keyValue);
            }

            return true;
        }

        /// <summary>
        /// [Required] Gets or sets the section. If you want to get the key/value pairs from the global section, use '[]' as the name of global section.
        /// </summary>
        /// <value>The section.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem Section { get; set; }

        /// <summary>
        /// Gets the internal section.
        /// </summary>
        /// <remarks></remarks>
        protected string InternalSection
        {
            get
            {
                if (Section.ItemSpec == GlobalSectionName)
                    return StreamIniDataParser.GlobalSectionName;
                return Section.ItemSpec;
            }
        }

        /// <summary>
        /// [Output] Gets the key/value pairs from <see cref="Section"/>. Each item has the following values.
        /// <list type="bullet">
        /// <item>
        /// <description>Identity : key name</description>
        /// </item>
        /// <item>
        /// <description>Metadata 'Value' : the value</description>
        /// </item>
        /// </list>
        /// <item>
        /// <description>Metadata 'Comments' : comments</description>
        /// </item>
        /// </summary>
        /// <remarks></remarks>
        [Output]
        public TaskItem[] KeyValues
        {
            get 
            { 
                return _keyValues.ToArray();
            }
        }

        /// <summary>
        /// Gets the key value.
        /// </summary>
        /// <param name="keyData">The key data.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private static TaskItem GetKeyValue(KeyData keyData)
        {
            TaskItem keyValue = new TaskItem(keyData.KeyName);
            keyValue.SetMetadata("Value", keyData.Value);

            string comments = GetComments(keyData);
            keyValue.SetMetadata("Comments", comments);
            return keyValue;
        }

        /// <summary>
        /// Gets the comments.
        /// </summary>
        /// <param name="keyData">The key data.</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private static string GetComments(KeyData keyData)
        {
            StringBuilder sb = new StringBuilder();
            foreach (string comment in keyData.Comments)
            {
                sb.AppendLine(comment);
            }
            return sb.ToString();
        }
    }
}
