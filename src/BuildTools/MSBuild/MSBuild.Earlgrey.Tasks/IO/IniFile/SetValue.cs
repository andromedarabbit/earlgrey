using System;
using System.Collections.Generic;
using System.Diagnostics;
using IniParser;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.IniFile
{
    public class SetValue : AbstractIniFile
    {
        public SetValue()
            : base()
        {
        }

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

        [Required]
        public string Value { get; set; }
    }
}
