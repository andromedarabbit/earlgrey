using System;
using System.Collections.Generic;
using IniParser;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{

    /// <summary>
    /// Get the corresponding value to the target key of the target section in the .ini file.
    /// </summary>
    /// <example>
    /// <code lang="ini" title="An ini file." source=".\Samples\Data\Sections.ini" />
    /// <code lang="xml" title="Getting the correspoing value." source=".\Samples\msbuild-inifile-getvalue.xml" />
    /// <code lang="xml" title="Getting the correspoing value from the global section." source=".\Samples\msbuild-inifile-getvalue-from-global-section.xml" />
    /// </example>
    /// <remarks></remarks>
    public class GetValue : AbstractIniFile
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="GetValue"/> class.
        /// </summary>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public GetValue()
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
            if (base.ExecuteCommand() == false)
                return false;

            if (Data.Sections.ContainsSection(InternalSection) == false)            
                throw new Exception();

            KeyDataCollection section = Data[InternalSection];
            if (section.ContainsKey(Key) == false)
                throw new Exception();

            Value = section[Key];

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
        /// <value>The section.</value>
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
        /// <value>The key.</value>
        /// <remarks></remarks>
        [Required]
        public string Key { get; set; }

        /// <summary>
        /// [Output] Gets or sets the corresponding value of the target key <see cref="Key"/> in the target section <see cref="Section"/> in the .ini file.
        /// </summary>
        /// <value>The value.</value>
        /// <remarks></remarks>
        [Output]
        public string Value { get; set; }
    }
}
