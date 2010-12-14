using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Microsoft.SqlServer.Management.Smo;
using Microsoft.SqlServer.Management.Smo.RegisteredServers;

namespace MSBuild.Earlgrey.Tasks.SqlServer2005
{
    internal static class RegisteredServerHelper
    {
        public static bool FindParentGroup(string path, out ServerGroup group)
        {
            try
            {
                group = FindParentGroup(path);
                return true;
            }
            catch (ApplicationException appEx)
            {
                group = null;
                if (appEx.Message == "Not Found!")
                    return false;
                throw appEx;
            }    
        }

        public static ServerGroup FindParentGroup(string path)
        {
            if(string.IsNullOrEmpty(path))
                return null;

            ServerGroupCollection collection = SmoApplication.SqlServerRegistrations.ServerGroups;
            List<ServerGroup> groups = FlatGroups(collection);

            return FindParentGroup(groups, path);
        }

        private static List<ServerGroup> FlatGroups(ServerGroupCollection collection)
        {
            List<ServerGroup> groups = new List<ServerGroup>(collection.Count);

            FlatGroups(collection, groups);
            return groups;
        }

        private static void FlatGroups(ServerGroupCollection collection, List<ServerGroup> groups)
        {
            foreach (ServerGroup group in collection)
            {
                groups.Add(group);
                if(group.ServerGroups == null)
                    continue;

                FlatGroups(group.ServerGroups, groups);
            }
        }

        private static ServerGroup FindParentGroup(List<ServerGroup> groups, string path)
        {
            var result =
                groups.Where(group => group.Path.Equals(path, StringComparison.CurrentCultureIgnoreCase) == true);

            if(result.Count() == 0)
                throw new ApplicationException("Not Found!");

            return result.First().Parent;
        }

        public static bool FindGroup(string path, string groupName, out ServerGroup group)
        {
            try
            {
                group = FindGroup(path, groupName);
                return true;
            }
            catch (ApplicationException appEx)
            {
                group = null;
                if (appEx.Message == "Not Found!")
                    return false;
                throw appEx;
            }
        }

        public static ServerGroup FindGroup(string path, string groupName)
        {
            if (string.IsNullOrEmpty(path) == false && string.IsNullOrEmpty(groupName))
                throw new ArgumentException();

            if (string.IsNullOrEmpty(path) && string.IsNullOrEmpty(groupName))
                return null;
            
            ServerGroupCollection collection = SmoApplication.SqlServerRegistrations.ServerGroups;
            List<ServerGroup> groups = FlatGroups(collection);

            return FindGroup(groups, path, groupName);
        }

        private static ServerGroup FindGroup(List<ServerGroup> groups, string path, string groupName)
        {
            IEnumerable<ServerGroup> result = SearchGroups(groups, path, groupName);

            if (result.Count() == 0)
                throw new ApplicationException("Not Found!");

            Debug.Assert(result.Count() == 1);
            return result.First();
        }

        private static IEnumerable<ServerGroup> SearchGroups(List<ServerGroup> groups, string path, string groupName)
        {   if(string.IsNullOrEmpty(path))
            {
                return groups.Where(
                    group => string.IsNullOrEmpty(group.Path) == true
                             && group.Name.Equals(groupName, StringComparison.CurrentCultureIgnoreCase) == true
                    ); 
            }

            return groups.Where(
                group => group.Path.Equals(path, StringComparison.CurrentCultureIgnoreCase) == true
                         && group.Name.Equals(groupName, StringComparison.CurrentCultureIgnoreCase) == true
                );
        }

    }
}
