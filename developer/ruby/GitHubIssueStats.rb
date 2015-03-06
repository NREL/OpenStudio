since = '2015-01-22'


require 'github_api'
require 'date'

repo_owner = 'NREL'
repo = 'OpenStudio'
date = Time.parse(since + 'T06:00:00Z')

github = Github.new

totalOpenIssues = Array.new
totalOpenPullRequests = Array.new
newIssues = Array.new
newOpenIssues = Array.new
closedIssues = Array.new
acceptedPullRequests = Array.new

def get_num(url)
  url.split('/')[-1].to_i
end

# Process Open Issues
results = -1
page = 1
while (results != 0)
  resp = github.issues.list user: repo_owner, repo: repo,
                            :sort => 'created',
                            :direction => 'asc',
                            :state => 'open',
                            :per_page => 100,
                            :page => page
  results = resp.length
  resp.env[:body].each do |issue, index|
    created = Time.parse(issue.created_at)
    if !issue.has_key?(:pull_request)
      totalOpenIssues << get_num(issue.html_url)
      if created >= date
        newIssues << get_num(issue.html_url)
      end
    else
      totalOpenPullRequests << get_num(issue.html_url)
    end
  end

  page = page + 1
end

newOpenIssues = newIssues.sort

# Process Closed Issues
results = -1
page = 1
while (results != 0)
  resp = github.issues.list user: repo_owner, repo: repo,
                            :sort => 'created',
                            :direction => 'asc',
                            :state => 'closed',
                            :per_page => 100,
                            :page => page
  results = resp.length
  resp.env[:body].each do |issue, index|
    created = Time.parse(issue.created_at)
    closed = Time.parse(issue.closed_at)
    if !issue.has_key?(:pull_request)
      if created >= date
        newIssues << get_num(issue.html_url)
      end
      if closed >= date
        closedIssues << get_num(issue.html_url)
      end
    elsif closed >= date
      acceptedPullRequests << get_num(issue.html_url)
    end
  end

  page = page + 1
end

newIssues.sort!

puts "Total Open Issues: #{totalOpenIssues.length}"
puts "Total Open Pull Requests: #{totalOpenPullRequests.length}"
puts "\nSince #{since}:"
puts "  Created Issues: #{newIssues.length} (Starting at ##{newIssues.first})"
puts "  Closed Issues: #{closedIssues.length} (" + closedIssues.join(', ') + ')'
puts "  Accepted Pull Requests: #{acceptedPullRequests.length}"

puts "\n"
puts "All Open Issues: #{totalOpenIssues.join(',')}"
newOpenIssues.each do |issue|
  puts "https://github.com/NREL/OpenStudio/issues/#{issue}"
end