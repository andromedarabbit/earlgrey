using System;
using System.Collections.Generic;
using System.Diagnostics;
using IniParser;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{

    /// <summary>
    /// Change the value of the target key or create a new key/value pair.
    /// </summary>
    /// <example>
    /// <code lang="ini" title="An ini file." source=".\Samples\Data\Sections.ini" />
    /// <code lang="xml" title="Change the correspoing value to the target key." source=".\Samples\msbuild-inifile-setvalue-change-old-one.xml" />
    /// <code lang="xml" title="Create a new key/value pair." source=".\Samples\msbuild-inifile-setvalue-create-new-one.xml" />
    /// </example>
    /// <remarks></remarks>
    public class SetValue : AbstractIniFile
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SetValue"/> class.
        /// </summary>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public SetValue()
            : base()
        {
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
            {
                Trace.Assert(Data.Sections.AddSection(InternalSection));
            }

            KeyDataCollection section = Data[InternalSection];
            if (section.ContainsKey(Key) == false)
            {
                Trace.Assert(section.AddKey(Key));
            }

            section[Key] = Value;

            Parser.SaveFile(FullPath, Data);
            
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
        /// [Required] Gets or sets the target section name.
        /// </summary>
        /// <value>The section name.</value>
        /// <remarks></remarks>
        [Required]
        public string Section { get; set; }

        /// <summary>
        /// Gets the internal section.
        /// </summary>
        /// <remarks></remarks>
        protected string InternalSection
        {
            get
            {
                if (Section == GlobalSectionName)
                    return StreamIniDataParser.GlobalSectionName;
                return Section;
            }
        }

        /// <summary>
        /// [Required] Gets or sets the target key name.
        /// </summary>
        /// <value>The key name.</value>
        /// <remarks></remarks>
        [Required]
        public string Key { get; set; }

        /// <summary>
        /// [Required] Gets or sets the value of the target key.
        /// </summary>
        /// <value>The value.</value>
        /// <remarks></remarks>
        [Required]
        public string Value { get; set; }
    }
}
