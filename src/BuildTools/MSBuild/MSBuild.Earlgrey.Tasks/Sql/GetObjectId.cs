using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Text;
using System.Linq;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Sql
{
    public class GetObjectId : AbstractGetId
    {
        protected override string GetCommandString(ITaskItem item)
        {
            return string.Format("SELECT OBJECT_ID(N'{0}')", item.ItemSpec);
        }

    }
}
