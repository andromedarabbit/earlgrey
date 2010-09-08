using System;
using System.Collections.Generic;
using IniParser;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{
    public class GetValue : AbstractIniFile
    {
        public GetValue()
            : base()
        {
        }

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

        protected override bool ValidateParameters()
        {
            return base.ValidateParameters();
        }

        [Required]
        public string Section { get; set; }

        protected string InternalSection
        {
            get
            {
                if (Section == GlobalSectionName)
                    return StreamIniDataParser.GlobalSectionName;
                return Section;
            }
        }

        [Required]
        public string Key { get; set; }

        [Output]
        public string Value { get; set; }
    }
}
