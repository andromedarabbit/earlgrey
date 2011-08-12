using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Text;
using System.Linq;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Sql
{
    public class GetObjectId : AbstractGetId
    {
        public GetObjectId()
            : base()
        {
            
        }

        protected override string GetCommandString(ITaskItem item)
        {
            return string.Format("SELECT OBJECT_ID(N'{0}')", item.ItemSpec);
        }

    }
}
